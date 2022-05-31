

//Task01-------------------Added by Me-------------------Task01//
#ifndef EVENTLISTENERSCREENSHOTS_H
#define EVENTLISTENERSCREENSHOTS_H

// Event listener for screenshots
// Dumps a new PNG file after the frame has been renderered (mostly at 5 frames per second)
namespace ve {
	class EventListenerScreenshots : public VEEventListener {
	private:
		double timePassed = 0;
		uint32_t numScreenshot = 0;  ///< Screenshot ID

	protected:
		virtual void onFrameEnded(veEvent event);

	public:
		EventListenerScreenshots(std::string name) : VEEventListener(name) {};
		virtual ~EventListenerScreenshots() {};
	};
}

#endif
//-----------------------------------------------------//