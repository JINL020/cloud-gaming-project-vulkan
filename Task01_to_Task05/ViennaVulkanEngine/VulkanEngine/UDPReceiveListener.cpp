
#include "VEInclude.h"
#include "UDPReceiveListener.h"

#include<thread>
#include "key.h"

std::string key = "";

namespace ve {
	
	void UDPReceiveListener::startReceiver() {

		receiver.startWinsock();
		receiver.init(8089);

		while (!getEnginePointer()->isEnd()) {
			char* data = new char[65000];
			double ptime;
			int ret = receiver.receive(data, sizeof data, &ptime);
			data[ret] = '\0';

			key = data;
			std::cout << "KEY: " << key << std::endl;
		}
	}

}
