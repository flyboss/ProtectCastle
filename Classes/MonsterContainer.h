#pragma once
#ifndef __ProtectCastle__MonsterContainer__
#define __ProtectCastle__MonsterContainer__
#include"MonsterDataOfWave.h"

#include "json/document.h"
#include "PointDelegate.h"

#include "Game.h"
extern class Game;
class MonsterContainer
{
public:


	MonsterContainer();
	~MonsterContainer();
	
	static  void loadMonster(Game& game, rapidjson::Value& monstersArray);

	static void updateMonster(float delta,Game& game,int monsterCount);
};

#endif