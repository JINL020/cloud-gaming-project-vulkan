

#include "UDPSender.h";

#ifndef LISTENERKEYBOARD_H
#define LISTENERKEYBOARD_H

class KeyboardListener {
private:
	UDPSender sender;

public:
	KeyboardListener();
	~KeyboardListener(){};
	void listen();
};

#endif
