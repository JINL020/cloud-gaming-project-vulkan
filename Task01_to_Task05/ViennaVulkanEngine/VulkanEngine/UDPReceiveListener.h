
// Game-------------------- - Added by Me-------------------- - Game//

#ifndef UDPREEIVELISTENER_H
#define UDPREEIVELISTENER_H

#include "UDPReceiver.h"

namespace ve {

	class UDPReceiveListener{
	private:
		UDPReceiver receiver;

	public:
		UDPReceiveListener(){};
		~UDPReceiveListener(){};
		void startReceiver();

	};

} // namespace ve

#endif

//-----------------------------------------------------//
