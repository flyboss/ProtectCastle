#ifndef __ProtectCastle__SingleAlgorithm__
#define __ProtectCastle__SingleAlgorithm__
#include "LaunchAlgorithm.h"
#include "bullet.h"
#include "Monster.h"
class SingleAlgorithm:public LaunchAlgorithm
{
private:



public:
	SingleAlgorithm();
	~SingleAlgorithm();
	void launch(TMXTiledMap* _tileMap, Monster* monster, Point turretPoint, Vector<Bullet*>& _bulletVector);


};





#endif
