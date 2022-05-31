#include "UDPReceiver.h"
#include <iostream>

int main() {
	UDPReceiver receiver;
	receiver.startWinsock();
	receiver.init(8088);

	//MPGDecoder decoder;
	//PNGEncoder encoder;

	char* buffer = new char[65000];
	double* ptime = new double(105);

	while (receiver.receive(buffer, 1, ptime))
	{
		size_t* data = (size_t*)(buffer);
		uint8_t* cdata = (uint8_t*)data;
		//decoder.decode(cdata, receivedBytes);
		//encoder.encode(cdata, 800, 600);
		puts("Reply received\n");
	}

}