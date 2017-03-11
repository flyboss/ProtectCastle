#ifndef __ProtectCastle__GameBoss__
#define __ProtectCastle__GameBoss__

#include "Game.h"

// Ã°ÏÕÄ£Ê½
class GameBoss : public Game
{
private:
	int _time;
public:
	static cocos2d::Scene* createSceneWithLevel(int selectLevel);
	bool init();
	void update(float delta);
	void updateMonster(float delta);
	void monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector, Monster* monster);
	void updateLable(float delta);
	CREATE_FUNC(GameBoss);
	void gameOver(int tag);
};

#endif