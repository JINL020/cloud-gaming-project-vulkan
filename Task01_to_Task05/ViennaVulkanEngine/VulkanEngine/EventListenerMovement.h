
// Game-------------------- - Added by Me-------------------- - Game//

#ifndef EVENTLISTENERMOVEMENT_H
#define EVENTLISTENERMOVEMENT_H

namespace ve {

	class EventListenerMovement : public VEEventListener {
	private:
	protected:
		virtual void onFrameEnded(veEvent event);

	public:
		EventListenerMovement(std::string name) : VEEventListener(name) { };
		virtual ~EventListenerMovement() {};
	};
} // namespace ve

#endif

//-----------------------------------------------------//
