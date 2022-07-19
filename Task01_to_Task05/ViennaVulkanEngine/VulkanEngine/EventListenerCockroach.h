
// Game-------------------- - Added by Me-------------------- - Game//

#ifndef EVENTLISTENERCOCKROACH_H
#define EVENTLISTENERCOCKROACH_H

namespace ve{

	class EventListenerCockroach : public VEEventListener{

	private:
		double timePassed = 0;
		float transSpeed = 8;
		glm::vec4 translate = glm::vec4(0.0, 0.0, -1.0, 1.0);
		float angle = 0.0;

	protected:
		virtual void onFrameEnded(veEvent event);

	public:
		///Constructor
		EventListenerCockroach(std::string name) : VEEventListener(name) { };

		///Destructor
		virtual ~EventListenerCockroach() {};
	};
} // namespace ve

#endif

//-----------------------------------------------------//
