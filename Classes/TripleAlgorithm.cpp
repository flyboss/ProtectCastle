#include "TripleAlgorithm.h"


TripleAlgorithm::TripleAlgorithm()
{
}

TripleAlgorithm::~TripleAlgorithm()
{
}

void TripleAlgorithm::launch(TMXTiledMap* _tileMap, Monster* monster, Point turretPoint, Vector<Bullet*>& _bulletVector)
{
	auto bullet1 = Bullet::createWithSpriteFrameName("bullet_01.png");
	auto bullet2 = Bullet::createWithSpriteFrameName("bullet_01.png");
	auto bullet3 = Bullet::createWithSpriteFrameName("bullet_01.png");
	bullet1->setScale(0.8);
	bullet2->setScale(0.8);
	bullet3->setScale(0.8);
	bullet1->setPosition(turretPoint.x, turretPoint.y);
	bullet2->setPosition(turretPoint.x, turretPoint.y);
	bullet3->setPosition(turretPoint.x, turretPoint.y);
	_tileMap->addChild(bullet1, 2);
	_tileMap->addChild(bullet2, 2);
	_tileMap->addChild(bullet3, 2);
	// 炮弹移动动作
	Vec2 right = Bullet::getBulletexX1(monster->getPosition(), turretPoint, 2);
	Vec2 left = Bullet::getBulletexX1(monster->getPosition(), turretPoint, 1);
	Vec2 middle = Bullet::getBulletexX1(monster->getPosition(), turretPoint, 0);
	// 计算炮弹移动的时间，避免因为距离长短而造成炮弹运行的速度问题
	float duration2 = Bullet::getBulletMoveTime(bullet1->getPosition(), right, _tileMap);
	float duration1 = Bullet::getBulletMoveTime(bullet1->getPosition(), left, _tileMap);
	float duration = Bullet::getBulletMoveTime(bullet1->getPosition(), middle, _tileMap);
	auto moveTo1 = MoveTo::create(duration, middle);
	auto moveTo2 = MoveTo::create(duration2, right);
	auto moveTo3 = MoveTo::create(duration1, left);
	bullet1->runAction(moveTo1);
	bullet2->runAction(moveTo2);
	bullet3->runAction(moveTo3);
	// 将炮弹添加到炮弹数组
	_bulletVector.pushBack(bullet1);
	_bulletVector.pushBack(bullet2);
	_bulletVector.pushBack(bullet3);
	bullet1->setShoot(true);
	bullet2->setShoot(true);
	bullet3->setShoot(true);
}