#pragma comment(lib, "ws2_32.lib")

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libavutil/common.h"
#include "libavutil/mathematics.h"
}

#include "UDPReceiver.h"

namespace ve {

    typedef struct RTHeader {
        double		  time;
        unsigned long packetnum;
        unsigned char fragments;
        unsigned char fragnum;
    } RTHeader_t;


    UDPReceiver::UDPReceiver() {
        sock = 0;
        recbuffer = new char[65000];
    }

    UDPReceiver::~UDPReceiver() {
        delete recbuffer;
        WSACleanup();
    }


    void UDPReceiver::startWinsock() {
        printf("Initialising Winsock...\n");
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0) {// 2, 0 or 2,2??? 
            printf("Failed. Error Code : %d", WSAGetLastError());
            return;
        }
        printf("Initialised Winsock.\n");
        return;
    }


    void UDPReceiver::init(int port) {
        if (sock) {
            closeSock();
        }

        printf("\nCreating socket...\n");
        if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
            printf("Could not create socket : %d", WSAGetLastError());
            return;
        }
        printf("Socket created.\n");

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
        int ret = bind(sock, (const sockaddr*)&addr, sizeof(addr));

        printf("\nBinding port %d return %d\n", port, ret);

        leftover = false;
    }


    int UDPReceiver::receive(char* buffer, int len, double* ptime) {
        return receive(buffer, len, "", ptime);
    }


    int UDPReceiver::receive(char* buffer, int len, const char* tag, double* ptime) {
        struct sockaddr_in si_other;
        socklen_t slen = sizeof(si_other);

        RTHeader_t* pheader = (RTHeader_t*)recbuffer;

        bool goon = true;
        int bytes = 0;
        int packetnum = -1;
        int fragments = -1;
        int fragnum = -1;
        int nextfrag = 1;

        while (goon) {

            int ret = 0;

            if (!leftover) {
                ret = recvfrom(sock, recbuffer, 65000, 0, (sockaddr*)&si_other, &slen);
            }
            leftover = false;

            //printf("%s UDP Packet %ld Size %d Fragment %d of %d Nextfrag %d\n", tag, pheader->packetnum, ret, pheader->fragnum, pheader->fragments, nextfrag );

            if (ret > sizeof(RTHeader_t)) {
                if (packetnum == -1) {						//first fragment of the new packet
                    packetnum = pheader->packetnum;
                }

                if (packetnum != pheader->packetnum) {		//last fragments lost
                    printf("Last Frag %d lost", nextfrag);
                    leftover = true;
                    return -1;
                }

                //printf("%s UDP Packet %ld Size %d Fragment %d of %d Nextfrag %d\n", tag, pheader->packetnum, ret, pheader->fragnum, pheader->fragments, nextfrag );

                if (nextfrag != pheader->fragnum) {			//a fragment is missing
                    printf("Fragment %d lost\n", nextfrag);
                    return -1;
                }
                nextfrag++;

                memcpy(buffer + bytes, recbuffer + sizeof(RTHeader_t), ret - sizeof(RTHeader_t));
                bytes += ret - sizeof(RTHeader_t);

                if (pheader->fragments == pheader->fragnum) goon = false;		//last fragment

                packetnum = pheader->packetnum;
                fragments = pheader->fragments;
                fragnum = pheader->fragnum;

                *ptime = pheader->time;

            }
            else {
                printf("Fragment %d not larger than %zd", pheader->fragnum, sizeof(RTHeader_t));
                return -1;
            }
        }


        leftover = false;

        return bytes;
    }


    void UDPReceiver::closeSock() {
        closesocket(sock);
        sock = 0;
    }

}