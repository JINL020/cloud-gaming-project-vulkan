
//Task04-------------------Added by Me-------------------Task04//


#ifndef EVENTLISTENERUDP_H
#define EVENTLISTENERUDP_H

namespace ve {
	class EventListenerUDP : public VEEventListener {
	private:
		double timePassed = 0;
		const char* filename;
		FILE* file;

	protected:
		virtual void onFrameEnded(veEvent event);

	public:
		EventListenerUDP(std::string name);
		virtual ~EventListenerUDP();
	};
}


#endif

//-------------------------Added by Me-------------------------//