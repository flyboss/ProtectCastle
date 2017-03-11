#ifndef __ProtectCastle__GameEndless__
#define __ProtectCastle__GameEndless__

#include "Game.h"
#include"MonsterContainer.h"

// ÎÞ¾¡Ä£Ê½
class GameEndless : public Game
{
private:
	int _sumNum;
public:
	static cocos2d::Scene* createSceneWithLevel(int selectLevel);
	bool init();
	void update(float delta);
	void updateMonster(float delta);
	void updateLable(float delta);
	CREATE_FUNC(GameEndless);
	void gameOver(int delta);
};

#endif