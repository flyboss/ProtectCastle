#ifndef __ProtectCastle__MultipleAlgorithm__
#define __ProtectCastle__MultipleAlgorithm__
#include "LaunchAlgorithm.h"
#include "bullet.h"
#include "Monster.h"
class MultipleAlgorithm :public LaunchAlgorithm
{
private:



public:
	MultipleAlgorithm();
	~MultipleAlgorithm();
	void launch(TMXTiledMap* _tileMap, Monster* monster, Point turretPoint, Vector<Bullet*>& _bulletVector);


};


#endif
