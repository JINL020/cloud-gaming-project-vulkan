
//Task03-------------------Added by Me-------------------Task03//


#ifndef EVENTLISTENERFFMPEG_H
#define EVENTLISTENERFFMPEG_H

namespace ve {
	class EventListenerFFMPEG : public VEEventListener {
	private:
		double timePassed = 0;
		const char* filename;
		FILE* file;

	protected:
		virtual void onFrameEnded(veEvent event);

	public:
		EventListenerFFMPEG(std::string name);
		virtual ~EventListenerFFMPEG();
	};
}


#endif

//-------------------------Added by Me-------------------------//