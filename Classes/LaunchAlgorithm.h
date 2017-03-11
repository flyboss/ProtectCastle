#ifndef __ProtectCastle__LaunchAlgorithm__
#define __ProtectCastle__LaunchAlgorithm__
#include "bullet.h"
#include "Monster.h"
class LaunchAlgorithm
{
private:

public:
	LaunchAlgorithm();
	~LaunchAlgorithm();
	virtual void launch(TMXTiledMap* _tileMap, Monster* monster, Point turretPoint, Vector<Bullet*>& _bulletVector)=0;
};



#endif