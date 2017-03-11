//
//  Bullet.cpp
//  ProtectPrincess
//
//  
//
//

#include "Bullet.h"

Bullet* Bullet::create(const std::string& filename)
{
	Bullet *sprite = new Bullet();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Bullet* Bullet::createWithSpriteFrameName(const std::string& filename)
{
	Bullet *sprite = new Bullet();
	if (sprite && sprite->initWithSpriteFrameName(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Bullet::setShoot(bool shoot)
{
	_shoot = shoot;
}

bool Bullet::isShoot()
{
	return _shoot;
}

void Bullet::settype(int type)
{
	_type = type;
}

int Bullet::gettype()
{
	return _type;
}


float Bullet::getBulletMoveTime(Vec2 start, Vec2 end, TMXTiledMap* _tileMap)
{
	// �������յ������ת��ΪTileMap����
	Vec2 tileStart = Coordinate::tileCoordForPosition(start, _tileMap);
	Vec2 tileEnd = Coordinate::tileCoordForPosition(end, _tileMap);
	// �ƶ�һ�������ʱ��
	float duration = 0.02f;
	// �����ƶ���������ƶ�ʱ��
	duration = duration * sqrtf((tileStart.x - tileEnd.x) * (tileStart.x - tileEnd.x)
		+ (tileStart.y - tileEnd.y) * (tileStart.y - tileEnd.y));
	return duration;
}



// �����ڵ�ʱ�����ڵ�����ת����
float Bullet::getTurretRotation(Point monsterPoint, Point turret)
{
	// ����ת�ĽǶȣ���Ҫʹ�����Ǵ����������� = ���� / �ڱ�
	// monsterPoint.x�����������x�ᣬturret->getPosition().x�����������x��
	int offX = monsterPoint.x - turret.x;
	// monsterPoint.y�����������y�ᣬturret->getPosition().y�����������y��
	int offY = monsterPoint.y - turret.y;
	// ��ת���� = atan2f(�����к���)������/�ڱߣ�
	float radian = atan2f(offY, offX);
	// CC_RADIANS_TO_DEGREES�������Խ�����ת��Ϊ�Ƕ�
	float degrees = CC_RADIANS_TO_DEGREES(radian);
	// ת�����ĽǶȺͱ����ڵ�ͼƬ���90�ȣ���ˣ�Ϊ�˵õ���ȷ�ķ������ǰ���Ҫ���������ת����
	return 90 - degrees;
}

Vec2 & Bullet::getBulletexX1(Vec2 monster, Vec2 turret, int num)
{

	int lengh = 1000;
	int x1 = monster.x; int y1 = monster.y;
	int x2 = turret.x;  int y2 = turret.y;
	float dis = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	float cos = (x1 - x2) / dis;
	float sin = (y1 - y2) / dis;
	float cosr = cos*sqrt(3) / 2 + sin * 0.5;
	float sinr = sin*sqrt(3) / 2 - cos * 0.5;
	float cosl = cos*sqrt(3) / 2 - sin * 0.5;
	float sinl = sin*sqrt(3) / 2 + cos * 0.5;
	if (num == 2) {
		int x = x2 + lengh*cosr;
		int y = y2 + lengh*sinr;
		return Vec2(x, y);
	}
	if (num == 1) {
		int x = x2 + lengh*cosl;
		int y = y2 + lengh*sinl;
		return Vec2(x, y);
	}
	if (num == 0) {
		int x = x2 + lengh*cos;
		int y = y2 + lengh*sin;
		return Vec2(x, y);
	}
	else return Vec2(0, 0);
}
