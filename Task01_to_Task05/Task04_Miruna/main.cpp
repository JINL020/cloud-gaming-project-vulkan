#include "MPGDecoder.h"
#include "UDPReceiver.h"
#include <iostream>

int main() {
	UDPReceiver receiver;
	receiver.startWinsock();
	receiver.init(8088);

	MPGDecoder decoder;
	PNGEncoder encoder;

	char* buffer = new char[65000];
	double* ptime = new double(105);

	int receivedBytes;
	do{
		receivedBytes = receiver.receive(buffer, 1, ptime);
		if(receivedBytes > 0){
			size_t* data = (size_t*)(buffer);
			uint8_t* cdata = (uint8_t*)data;
			decoder.decode(cdata, receivedBytes);
			encoder.encode(cdata, 800, 600);
		}
	} while (receivedBytes != 0);

}