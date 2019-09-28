#include "common.h"

GameRule *GameRule::ss = nullptr;
GameRule::GameRule(){
	maxLife = 3;
	startStage = 1;
	breakRule = 1;
}

int GameRule::getMaxLife(){
	return maxLife;
}

int GameRule::getStartStage(){
	return startStage;
}

int GameRule::getBreakRule(){
	return breakRule;
}

void GameRule::setMaxLife(int x){
	maxLife = x;
}

void GameRule::setStartStage(int x){
	startStage = x;
}

void GameRule::setBreakRule(int x){
	breakRule = x;
}

GameRule *GameRule::getInstance(){
	if(GameRule::ss == nullptr) return GameRule::ss = new GameRule;
	else return GameRule::ss;
}
