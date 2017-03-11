#include "SingleAlgorithm.h"


SingleAlgorithm::SingleAlgorithm()
{
}

SingleAlgorithm::~SingleAlgorithm()
{
}

void SingleAlgorithm::launch(TMXTiledMap* _tileMap, Monster* monster, Point turretPoint ,Vector<Bullet*>& _bulletVector)
{
	auto bullet = Bullet::createWithSpriteFrameName("bullet_01.png");
	bullet->setScale(0.8);
	bullet->setPosition(turretPoint.x, turretPoint.y);
	_tileMap->addChild(bullet, 2);
	Vec2 middle = Bullet::getBulletexX1(monster->getPosition(), turretPoint, 0);
	float duration = Bullet::getBulletMoveTime(bullet->getPosition(), middle, _tileMap);
	auto moveTo = MoveTo::create(duration, middle);
	bullet->runAction(moveTo);
	bullet->setShoot(true);
	_bulletVector.pushBack(bullet);
}
