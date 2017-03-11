#include "MultipleAlgorithm.h"

MultipleAlgorithm::MultipleAlgorithm()
{
}

MultipleAlgorithm::~MultipleAlgorithm()
{
}

void MultipleAlgorithm::launch(TMXTiledMap* _tileMap, Monster* monster, Point turretPoint, Vector<Bullet*>& _bulletVector)
{
	monster->setLifeValue(monster->getLifeValue() - 1);
}
