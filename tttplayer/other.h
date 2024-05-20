#pragma once
#include "game_state.h"

#define line printf_s("%s", "================================================== \n") 
//#define n printf_s("\n") 
#define rep(n) for(int i=0; i<n; i++)
#define rep2(n) for(int j=0; j<n; j++)
#define rep3(n) for(int k=0; k<n; k++)


union Messages {
	char msgType; //1 == GameState, 2 == GameSettings, 3 == Event;
	GameState state;
	GameSettings settings;
	Event event;
};
