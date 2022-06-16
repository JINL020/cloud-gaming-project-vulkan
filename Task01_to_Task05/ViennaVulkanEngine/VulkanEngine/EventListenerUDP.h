
//Task04-------------------Added by Me-------------------Task04//
#pragma comment(lib, "ws2_32.lib")

extern "C" {
#define NOMINMAX
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <ws2ipdef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <WS2tcpip.h>
#include <cstdint>
}

#include "UDPSend.h"

#ifndef EVENTLISTENERUDP_H
#define EVENTLISTENERUDP_H

namespace ve {

	class EventListenerUDP : public VEEventListener {
	private:
		double timePassed = 0;
		const char* filename;
		FILE* file;
		UDPSend sender;

	protected:
		virtual void onFrameEnded(veEvent event);

	public:
		EventListenerUDP(std::string name);
		virtual ~EventListenerUDP();
		void startWinsock();
	};
}


#endif

//-------------------------Added by Me-------------------------//