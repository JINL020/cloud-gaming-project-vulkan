#define _CRT_SECURE_NO_DEPRECATE

#include <opencv2/highgui.hpp>

#include "UDPReceiver.h"
#include "decoder.h"
#include "keyboardListener.h"

#include<thread>


void keyboardListener() {
    KeyboardListener keyboardListener;
    keyboardListener.listen();
}


int main(int argc, char** argv) {
    UDPReceiver receiver;
    receiver.startWinsock();
    receiver.init(8088);


    cv::namedWindow("Is that a cockroach I see?...", cv::WINDOW_NORMAL);
    cv::resizeWindow("Is that a cockroach I see?...", 800, 600);


    std::thread worker(keyboardListener);
    

    Decoder decoder;

    int receivedBytes = 0;
    do {

        char* data = new char[65000];
        //double* ptime = new double[100];
        double ptime;

        receivedBytes = receiver.receive(data, sizeof data, &ptime);

        decoder.decode((uint8_t*)data, receivedBytes);
        //printf("received %i Bytes\n", receivedBytes);

    } while (receivedBytes != 0);

    puts("no bytes received");

    cv::destroyAllWindows();

    return 0;
}