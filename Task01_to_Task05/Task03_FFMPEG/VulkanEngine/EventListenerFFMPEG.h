/**
	@author Jin-Jin Lee
*/


#ifndef EVENTLISTENERFFMPEG_H
#define EVENTLISTENERFFMPEG_H

#include "UDPSend.h"

namespace ve {
	class EventListenerFFMPEG : public VEEventListener {
	private:
		double timePassed = 0;
		const char* filename = "media/video/task03_video_25Framerate_noLoop.mpg";
		FILE* file;
		UDPSend udpSender;

	protected:
		virtual void onFrameEnded(veEvent event);

	public:
		EventListenerFFMPEG(std::string name);
		virtual ~EventListenerFFMPEG();
	};
}


#endif
