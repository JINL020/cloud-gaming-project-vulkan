Task01
- Added EventListenerScreenshots.h and EventListenerScreenshots.cpp
- Modified main.cpp: registered new EventListenerScreenshots and added EventListenerScreenshots.h


Task02
- Modified main.cpp: loads in a pacman and changed collision to pacman and not camera anymore
- Modified VEEventListenerGLFW.h and VEEventListenerGLFW.cpp to enable pacman movement


Task03
- Added EventListenerFFPMEG.h and EventListenerFFMPEG.cpp
- Modified main.cpp: registered new EventListenerFFMEG and added EventListenerFFMPEG.h and extern "C"
- Added video folder in ViennaVulkanEngine\VulkanEngine\media


Task04
- Added UDPSend.h and UDPSend.cpp
- Modified main.cpp: added startWinsock(), #pragma comment and winsock2.h
- 


Task05
- Added EventListenerMyGUI.h and EventListenerMyGUI.cpp
- Modified main.cpp: registered new EventListenerMyGUI and disabled all other EventListeners and added EventListenerMyGUI.h
- Modified VESceneManager.h and VESceneManager.cpp: added getSceneNodes()


Game
- Modified main.cpp: loaded in room, player, cockroach, replaced box with cockroach; added EventListenerCockroach.h, registered new EventListenerCockroach
- Modified VEEngine.cpp: changed camera postion and pan
- Changed Pacman to Player
- Models: fairy and cockroach are from https://www.turbosquid.com/ and room is from me (wood texture from https://pixabay.com/)
- Modified VEEventListenerGLFW: player moves in facing direction with only arrow keys
- Added EventListenerCockroach: cockroach moves in random pattern