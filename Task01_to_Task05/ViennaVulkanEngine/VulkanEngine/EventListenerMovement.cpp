
//Game---------------------Added by Me---------------------Game//

#include "VEInclude.h"
#include "EventListenerMovement.h"
#include "key.h"


namespace ve {

	void EventListenerMovement::onFrameEnded(veEvent event) {

		VESceneNode* pPlayer = getSceneManagerPointer()->getSceneNode("Player Parent")->getChildrenList().at(0);
		glm::mat4 playerRot = pPlayer->getRotation();
		glm::vec3 playerPos = pPlayer->getPosition();

		float angle = 0.0;
		glm::vec4 translate = glm::vec4(0.0, 0.0, -1.0, 1.0);

		if (key == "ESC") {
			//getEnginePointer()->end();
			exit(0);
		}
		else if (key == "UP") {
			angle = (180 * M_PI) / 180;
		}
		else if (key == "DOWN") {
			angle = (0 * M_PI) / 180;
		}
		else if (key == "LEFT") {
			angle = (90 * M_PI) / 180;
		}
		else if (key == "RIGHT") {
			angle = (-90 * M_PI) / 180;
		}
		else {
			key = "";
			return;
		}


		glm::mat4  rotate = glm::rotate(glm::mat4(1.0), angle, glm::vec3(0, 1, 0));
		pPlayer->setTransform(rotate);

		float speed = 15.0f;
		glm::vec4 trans = (float) event.dt * playerRot * speed * translate;
		pPlayer->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(trans.x, trans.y, trans.z) + playerPos));


		if (playerPos.x < -8) {
			pPlayer->setPosition(glm::vec3(-8, playerPos.y, playerPos.z));
		}
		if (playerPos.x > 8) {
			pPlayer->setPosition(glm::vec3(8, playerPos.y, playerPos.z));
		}
		if (playerPos.z < -8) {
			pPlayer->setPosition(glm::vec3(playerPos.x, playerPos.y, -8));
		}
		if (playerPos.z > 8) {
			pPlayer->setPosition(glm::vec3(playerPos.x, playerPos.y, 8));
		}

		key = "";
	}

} // namespace ve

//-----------------------------------------------------//
