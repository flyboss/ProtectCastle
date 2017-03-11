#include "Turret.h"

Turret* Turret::create(const std::string& filename)
{
	Turret *sprite = new Turret();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Turret* Turret::createWithSpriteFrameName(const std::string& filename)
{
	Turret *sprite = new Turret();
	if (sprite && sprite->initWithSpriteFrameName(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

// 检测是否在圆攻击范围内
bool Turret::checkPointInCircle(Vec2 monsterPoint, Vec2 turretPoint, int area)
{
	int x = monsterPoint.x - turretPoint.x;
	int y = monsterPoint.y - turretPoint.y;
	// sqrt函数求平方根
	if (sqrt(x * x + y * y) <= area) return true;
	return false;
}

void Turret::setBulletName(std::string bulletName)
{
	_bulletName = bulletName;
}

void Turret::setSelect(bool select)
{
	_select = select;
}

bool Turret::isSelect()
{
	return _select;
}

void Turret::setBullet(Bullet* bullet)
{
	_bullet = bullet;
}

Bullet* Turret::getBullet()
{
	return _bullet;
}

void Turret::setTime(int time)
{
	_time = time;
}

int Turret::getTime()
{
	return _time;
}

std::string Turret::getBulletName()
{
	return _bulletName;
}

void Turret::setType(int type)
{
	_mtype = type;
	setALA();
}

int Turret::getType()
{
	return _mtype;
}


void Turret::Launch(TMXTiledMap* _tileMap, Monster* monster, Vector<Bullet*>& _bulletVector)
{
	_LAL->launch(_tileMap, monster, this->getPosition(), _bulletVector);
}
LaunchAlgorithm* Turret::getALA()
{
	return _LAL;
}
void Turret::setALA()
{
	if (_mtype == 0)
	{
		_LAL = new SingleAlgorithm();
	}
	else if (_mtype == 1)
	{
		_LAL = new TripleAlgorithm();
	}
	else
	{
		_LAL = new MultipleAlgorithm();
	}
}
