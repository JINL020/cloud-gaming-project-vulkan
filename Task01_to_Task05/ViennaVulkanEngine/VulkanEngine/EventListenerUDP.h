
//Task04-------------------Added by Me-------------------Task04//

extern "C" {
#define NOMINMAX
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <in6addr.h>
#include <ws2ipdef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <WS2tcpip.h>
#include <cstdint>
}

#ifndef EVENTLISTENERUDP_H
#define EVENTLISTENERUDP_H

namespace ve {
	void startWinsock();

	class EventListenerUDP : public VEEventListener {
	private:
		double timePassed = 0;
		const char* filename;
		FILE* file;
		//UDPSend sender;

	protected:
		virtual void onFrameEnded(veEvent event);

	public:
		EventListenerUDP(std::string name);
		virtual ~EventListenerUDP();
	};
}


#endif

//-------------------------Added by Me-------------------------//