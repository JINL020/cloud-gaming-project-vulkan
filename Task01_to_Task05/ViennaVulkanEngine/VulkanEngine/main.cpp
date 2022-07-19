/**
* The Vienna Vulkan Engine
*
* (c) bei Helmut Hlavacs, University of Vienna
*
*/

#include "VEInclude.h"

#include "EventListenerScreenshots.h" //Added by Me Task01
#include "EventListenerFFMPEG.h" //Added by Me Task03
#include "EventListenerUDP.h" //Added by Me Task04
#include "EventListenerMyGUI.h" //Added by Me Task05

#include "UDPReceiveListener.h" // Game
#include "EventListenerCockroach.h" // Game
#include "EventListenerMovement.h" // Game

#include<cmath>
#include<thread>

#include<cmath>


namespace ve {

	double g_highscore = 1000000000000;

	uint32_t g_score = 10;				//derzeitiger Punktestand
	double g_time = 0.0;				//zeit die noch übrig ist
	bool g_gameLost = false;			//true... das Spiel wurde verloren
	bool g_restart = false;			//true...das Spiel soll neu gestartet werden

	bool g_start = true;

	//
	//Zeichne das GUI
	//
	class EventListenerGUI : public VEEventListener {
	protected:
		
		virtual void onDrawOverlay(veEvent event) {
			VESubrender_Nuklear * pSubrender = (VESubrender_Nuklear*)getEnginePointer()->getRenderer()->getOverlay();
			if (pSubrender == nullptr) return;

			struct nk_context * ctx = pSubrender->getContext();

			VkExtent2D extent = getWindowPointer()->getExtent();
			uint32_t centerHeight = extent.height / 2;
			uint32_t centerWidth = extent.width / 2;

			if (g_start) {
				if (nk_begin(ctx, "Is that a cockroach...", nk_rect(centerWidth - 150, centerHeight - 100, 300, 200), NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
					nk_layout_row_dynamic(ctx, 45, 1);
					if (nk_button_label(ctx, "start game")) {
						getEnginePointer()->m_irrklangEngine->play2D("media/sounds/ophelia.wav", true);
						g_start = false;

					}

				}
			}
			else if (!g_gameLost) {
				if (nk_begin(ctx, "", nk_rect(0, 0, 200, 170), NK_WINDOW_BORDER )) {
					char outbuffer[100];
					nk_layout_row_dynamic(ctx, 45, 1);
					sprintf(outbuffer, "Left to kill: %02d", g_score);
					nk_label(ctx, outbuffer, NK_TEXT_LEFT);

					nk_layout_row_dynamic(ctx, 45, 1);
					sprintf(outbuffer, "Time: %004.1lf", g_time);
					nk_label(ctx, outbuffer, NK_TEXT_LEFT);

					if (nk_button_label(ctx, "Restart")) {
						g_gameLost = true;
						getEnginePointer()->m_irrklangEngine->removeAllSoundSources();
						getEnginePointer()->m_irrklangEngine->play2D("media/sounds/explosion.wav", false);
						g_restart = true;
					}

				}
			}
			else {
				if (nk_begin(ctx, "", nk_rect(centerWidth - 150, centerHeight - 100, 300, 250), NK_WINDOW_BORDER)) {
					nk_layout_row_dynamic(ctx, 45, 1);
					nk_label(ctx, "You won!", NK_TEXT_LEFT);
					
					char outbuffer[100];
					sprintf(outbuffer, "Highscore: %004.1lf", g_highscore);
					nk_label(ctx, outbuffer, NK_TEXT_LEFT);

					sprintf(outbuffer, "Your time: %004.1lf", g_time);
					nk_label(ctx, outbuffer, NK_TEXT_LEFT);

					if (nk_button_label(ctx, "play again")) {
						g_restart = true;
					}
				}

			};

			nk_end(ctx);
		}

	public:
		///Constructor of class EventListenerGUI
		EventListenerGUI(std::string name) : VEEventListener(name) { };

		///Destructor of class EventListenerGUI
		virtual ~EventListenerGUI() {};
	};


	static std::default_random_engine e{ 12345 };					//Für Zufallszahlen
	static std::uniform_real_distribution<> d{ -8.0f, 8.0f };		//Für Zufallszahlen

	//
	// Überprüfen, ob die Kamera die Kiste berührt
	//
	//Task02------------------Modfied by Me------------------Task02//
	class EventListenerCollision : public VEEventListener {
	protected:
		virtual void onFrameStarted(veEvent event) {
			if (g_start) {
				return;
			}

			if (g_restart) {
				g_gameLost = false;
				g_restart = false;
				g_time = 00;
				g_score = 10;
				getSceneManagerPointer()->getSceneNode("Cockroach Parent")->setPosition(glm::vec3(d(e), 2.0f, d(e)));
				getEnginePointer()->m_irrklangEngine->play2D("media/sounds/ophelia.wav", true);
				return;
			}
			if (g_gameLost) return;

			glm::vec3 positionCube   = getSceneManagerPointer()->getSceneNode("Cockroach Parent")->getPosition();
			glm::vec3 positionPlayer = getSceneManagerPointer()->getSceneNode("Player")->getPosition();

			float distance = glm::length(positionCube - positionPlayer);
			if (distance < 2) {
				g_score--;
				getEnginePointer()->m_irrklangEngine->play2D("media/sounds/explosion.wav", false);
				/*if (g_score % 10 == 0) {
					g_time = 0;
					getEnginePointer()->m_irrklangEngine->play2D("media/sounds/bell.wav", false);
				}*/

				VESceneNode * cockroachParent = getSceneManagerPointer()->getSceneNode("Cockroach Parent");
				cockroachParent->setPosition(glm::vec3(d(e), 2.0f, d(e)));
			}

			g_time += event.dt;
			if (g_score == 0) {
				g_gameLost = true;
				getEnginePointer()->m_irrklangEngine->removeAllSoundSources();
				getEnginePointer()->m_irrklangEngine->play2D("media/sounds/bell.wav", false);
				if (g_time < g_highscore) {
					g_highscore = g_time;
				}
			}
		};

	public:
		///Constructor of class EventListenerCollision
		EventListenerCollision(std::string name) : VEEventListener(name) { };

		///Destructor of class EventListenerCollision
		virtual ~EventListenerCollision() {};
	};
	//---------------------------------------------------------------//

	

	///user defined manager class, derived from VEEngine
	class MyVulkanEngine : public VEEngine {
	public:

		MyVulkanEngine(veRendererType type = veRendererType::VE_RENDERER_TYPE_FORWARD, bool debug=false) : VEEngine(type, debug) {};
		~MyVulkanEngine() {};


		///Register an event listener to interact with the user
		
		virtual void registerEventListeners() {
			//VEEngine::registerEventListeners();

			registerEventListener(new EventListenerCollision("Collision"), { veEvent::VE_EVENT_FRAME_STARTED });
			registerEventListener(new EventListenerGUI("GUI"), { veEvent::VE_EVENT_DRAW_OVERLAY });
			
			//Task01-Task05----------------------Added by Me----------------------Task01-Task05//
			//registerEventListener(new EventListenerScreenshots("Screenshots"), { veEvent::VE_EVENT_FRAME_ENDED });
			//registerEventListener(new VEEventListenerNuklear("Nuklear"), { veEvent::VE_EVENT_DRAW_OVERLAY});
			//registerEventListener(new VEEventListenerNuklearDebug("Nuklear_debug"), { veEvent::VE_EVENT_DRAW_OVERLAY });
			//registerEventListener(new EventListenerFFMPEG("FFMPEG"), { veEvent::VE_EVENT_FRAME_ENDED });
			registerEventListener(new EventListenerUDP("UDP Send"), { veEvent::VE_EVENT_FRAME_ENDED });
			//registerEventListener(new EventListenerMyGUI("MyGUI"), { veEvent::VE_EVENT_DRAW_OVERLAY });
			//-------------------------------------------------------------------------------//
			registerEventListener(new EventListenerCockroach("CockroachMovement"), { veEvent::VE_EVENT_FRAME_ENDED }); // Game
<<<<<<< HEAD
			registerEventListener(new EventListenerMovement("Movement"), { veEvent::VE_EVENT_FRAME_ENDED }); // Game
=======
>>>>>>> 322edc4317c2bb62eedb011d537c8fd21430e40f
		};
		

		///Load the first level into the game engine
		///The engine uses Y-UP, Left-handed
		virtual void loadLevel( uint32_t numLevel=1) {

			VEEngine::loadLevel(numLevel );			//create standard cameras and lights

			VESceneNode *pScene;
			VECHECKPOINTER( pScene = getSceneManagerPointer()->createSceneNode("Level 1", getRoot()) );
	
			//scene models

			VESceneNode *sp1;
			VECHECKPOINTER( sp1 = getSceneManagerPointer()->createSkybox("The Sky", "media/models/test/sky/cloudy",
										{	"bluecloud_ft.jpg", "bluecloud_bk.jpg", "bluecloud_up.jpg", 
											"bluecloud_dn.jpg", "bluecloud_rt.jpg", "bluecloud_lf.jpg" }, pScene)  );

			VESceneNode *e4;
			VECHECKPOINTER( e4 = getSceneManagerPointer()->loadModel("The Plane", "media/models/test/plane", "plane_t_n_s.obj",0, pScene) );
			e4->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1.0f, 1000.0f)));

			VEEntity *pE4;
			VECHECKPOINTER( pE4 = (VEEntity*)getSceneManagerPointer()->getSceneNode("The Plane/plane_t_n_s.obj/plane/Entity_0") );
			pE4->setParam( glm::vec4(1000.0f, 1000.0f, 0.0f, 0.0f) );

			VESceneNode *cockroachParent;
			cockroachParent = getSceneManagerPointer()->createSceneNode("Cockroach Parent", pScene, glm::mat4(1.0));
			VESceneNode * cockroach;
			VECHECKPOINTER(cockroach = getSceneManagerPointer()->loadModel("Cockroach", "media/models/cockroach", "cockroach.obj"));
			cockroachParent->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 2.0f, 8.0f)));
			cockroachParent->addChild(cockroach);

			VESceneNode* roomParent;
			roomParent = getSceneManagerPointer()->createSceneNode("Room Parent", pScene, glm::mat4(1.0));
			VESceneNode* room;
			VECHECKPOINTER(room = getSceneManagerPointer()->loadModel("Room", "media/models/room", "room.obj", 0, roomParent));
			room->setPosition(glm::vec3(0.0f, 1.5f, 0.0f));
			roomParent->addChild(room);


			//Task02-------------------Added by Me-------------------Task02//
			VESceneNode* playerParent;
			playerParent = getSceneManagerPointer()->createSceneNode("Player Parent", pScene, glm::mat4(1.0));
			VESceneNode* player;
			VECHECKPOINTER(player = getSceneManagerPointer()->loadModel("Player", "media/models/fairy", "fairy.obj", 0, playerParent));
			player->setPosition(glm::vec3(0.0f, 1.5f, 0.0f));
			playerParent->addChild(player);
			//-------------------------------------------------------------//

			//m_irrklangEngine->play2D("media/sounds/ophelia.wav", true);
		};
	};

	void receive() {
		UDPReceiveListener movementController;
		movementController.startReceiver();
	}
}


using namespace ve;

int main() {
	bool debug = true;
	
	MyVulkanEngine mve(veRendererType::VE_RENDERER_TYPE_FORWARD, debug);	//enable or disable debugging (=callback, validation layers)

	mve.initEngine();
	mve.loadLevel(1);

	std::thread worker(receive);

	mve.run();

	worker.join();

	return 0;
}
