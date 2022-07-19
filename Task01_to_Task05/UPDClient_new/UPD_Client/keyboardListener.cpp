#define _CRT_SECURE_NO_DEPRECATE

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESC 27


#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<iostream>

#include "keyboardListener.h"


KeyboardListener::KeyboardListener(){
	sender.startWinsock();
	sender.init("127.0.0.1", 8089);
}


void KeyboardListener::listen(){
	char buf[100];

	int key = _getch();

	while (key != KEY_ESC){	
		key = _getch();
		switch (key) {
		case KEY_UP:
			sprintf(buf, "UP");
			sender.send(buf, strlen(buf));
			puts("up");
			break;
		case KEY_DOWN:
			sprintf(buf, "DOWN");
			sender.send(buf, strlen(buf));
			puts("down");
			break;
		case KEY_LEFT:
			sprintf(buf, "LEFT");
			sender.send(buf, strlen(buf));
			puts("left");
			break;
		case KEY_RIGHT:
			sprintf(buf, "RIGHT");
			sender.send(buf, strlen(buf));
			puts("rigth");
			break;
		}
		
	}	

	sprintf(buf, "ESC");
	sender.send(buf, strlen(buf));
	puts("Closed game");
	exit(0);
}
