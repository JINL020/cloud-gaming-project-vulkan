/**
	@author Jin-Jin Lee
*/


#ifndef VEEVENTLISTENERSCREENSHOT_H
#define VEEVENTLISTENERSCREENSHOT_H

/**
*
* \brief An event listener for screenshots
*
* Dumps a new PNG file after the frame has been renderered, mostly at 5 frames per second
*
*/
namespace ve {
	class VEEventListenerScreenshot : public VEEventListener {
	private:
		double timePassed = 0;
		uint32_t numScreenshot = 0;  ///< Screenshot ID

	protected:
		virtual void onFrameEnded(veEvent event);

	public:
		VEEventListenerScreenshot(std::string name) : VEEventListener(name) {};
		virtual ~VEEventListenerScreenshot() {};
	};
}


#endif
