#ifndef __ProtectPrincess__Bullet__
#define __ProtectPrincess__Bullet__

#include <iostream>
#include "cocos2d.h"
#include "SpriteBase.h"
#include "Coordinate.h"
// �ڵ���
class Bullet : public SpriteBase
{
private:
	bool _shoot = false; // �ڵ��Ƿ���
	int _type = 0;
public:
	static Bullet* create(const std::string& filename);

	static Bullet* createWithSpriteFrameName(const std::string& filename);

	void setShoot(bool shoot);

	bool isShoot();

	void settype(int type);

	int gettype();

	// �����ڵ��ƶ���������ڵ���Ҫ�ƶ�ʱ��
	static float getBulletMoveTime(Vec2 start, Vec2 end, TMXTiledMap* _tileMap);
	// ���㷢���ڵ�ʱ�����ڵ�����ת����
	static float getTurretRotation(Point monsterPoint, Point turret);

	static Vec2 & getBulletexX1(Vec2 monster, Vec2 turret, int num);
};

#endif