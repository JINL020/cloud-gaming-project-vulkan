
//Game---------------------Added by Me---------------------Game//

#include "VEInclude.h"
#include "EventListenerCockroach.h"
#include<cmath>
#include <cstdlib>

namespace ve {

	static std::default_random_engine e{ 12345 };					//F�r Zufallszahlen
<<<<<<< HEAD
	static std::uniform_real_distribution<> randomPos{ -8.0f, 8.0f };
	static std::uniform_real_distribution<> randomAngle{ -0, 180.0f };
=======
	static std::uniform_real_distribution<> d{ -8.0f, 8.0f };
	static std::uniform_real_distribution<> randomAngle{ -0, 180.0f };
	static std::uniform_real_distribution<> randomSpeed{ 20.0f, 40.0f };
>>>>>>> 322edc4317c2bb62eedb011d537c8fd21430e40f


	void EventListenerCockroach::onFrameEnded(veEvent event) {
		timePassed += event.dt;

		if (timePassed >= 0.05) {

			VESceneNode* pCockroach = getSceneManagerPointer()->getSceneNode("Cockroach Parent");
			glm::mat4 cockroachRot = pCockroach->getRotation();
			glm::vec3 cockroachPos = pCockroach->getPosition();

			angle = 0.0;

			if (cockroachPos.x < -8 || cockroachPos.x > 8 || cockroachPos.z < -8 || cockroachPos.z > 8) {
				angle = (randomAngle(e) * M_PI) / 180;
			}

			glm::mat4  rotate = glm::rotate(glm::mat4(1.0), angle, glm::vec3(0, 1, 0));
			pCockroach->setTransform(rotate * cockroachRot);

			
			glm::vec4 trans = (float)event.dt * cockroachRot * transSpeed * translate;
			pCockroach->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(trans.x, trans.y, trans.z) + cockroachPos));

			if (cockroachPos.x < -8 || cockroachPos.x > 8 || cockroachPos.z < -8 || cockroachPos.z > 8) {
				pCockroach->setPosition(glm::vec3(randomPos(e), 2.0f, randomPos(e)));
			}
			
			timePassed = 0;
		}
	}

} // namespace ve

//-----------------------------------------------------//