#ifndef __ProtectCastle__TripleAlgorithm__
#define __ProtectCastle__TripleAlgorithm__
#include "LaunchAlgorithm.h"
#include "bullet.h"
#include "Monster.h"
class TripleAlgorithm :public LaunchAlgorithm
{
private:



public:
	TripleAlgorithm();
	~TripleAlgorithm();
	void launch(TMXTiledMap* _tileMap, Monster* monster, Point turretPoint, Vector<Bullet*>& _bulletVector);


};


#endif
