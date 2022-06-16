#ifndef VEEVENTLISTENERFRAME_H
#define VEEVENTLISTENERFRAME_H


namespace ve {
    class VEEventListenerFrame : public VEEventListener {
    protected:
		MPGEncoder encoder;
		
		bool  encodeFrame = false;
		double totalTime = 0;

        virtual void onFrameEnded(veEvent event);
    public:
		VEEventListenerFrame(std::string name) : VEEventListener(name) {
		};
		virtual ~VEEventListenerFrame() {
		};
    };
}


# endif