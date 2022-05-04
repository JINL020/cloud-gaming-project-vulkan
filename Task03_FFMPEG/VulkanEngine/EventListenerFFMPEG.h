/**
	@author Jin-Jin Lee
*/


#ifndef EVENTLISTENERFFMPEG_H
#define EVENTLISTENERFFMPEG_H

/**
*
* \brief An event listener for screenshots
*
* 
*
*/
namespace ve {
	class EventListenerFFMPEG : public VEEventListener {
	private:
		double timePassed = 0;
		//FILE* file = fopen("media/frames/task03_video.mpg", "wb");

	protected:
		virtual void onFrameEnded(veEvent event);

	public:
		EventListenerFFMPEG(std::string name) : VEEventListener(name) {};
		virtual ~EventListenerFFMPEG() {};
	};
}


#endif