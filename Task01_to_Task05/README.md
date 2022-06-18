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
- Modified main.cpp:
        - added startWinsock(), #pragma comment and winsock2.h
        - loaded in room, player, cockroach
        - 
- Modified VEInclude.h: added UDPSend.h and EventListenerUDP.h
- Modified VEEngine.cpp: changed camera postion and pan
- 


Task05
- Added EventListenerMyGUI.h and EventListenerMyGUI.cpp
- Modified main.cpp: registered new EventListenerMyGUI and disabled all other EventListeners and added EventListenerMyGUI.h
- Modified VESceneManager.h and VESceneManager.cpp: added getSceneNodes()