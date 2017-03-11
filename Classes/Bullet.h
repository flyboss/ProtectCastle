#ifndef __ProtectPrincess__Bullet__
#define __ProtectPrincess__Bullet__

#include <iostream>
#include "cocos2d.h"
#include "SpriteBase.h"
#include "Coordinate.h"
// 炮弹类
class Bullet : public SpriteBase
{
private:
	bool _shoot = false; // 炮弹是否发射
	int _type = 0;
public:
	static Bullet* create(const std::string& filename);

	static Bullet* createWithSpriteFrameName(const std::string& filename);

	void setShoot(bool shoot);

	bool isShoot();

	void settype(int type);

	int gettype();

	// 根据炮弹移动距离计算炮弹需要移动时间
	static float getBulletMoveTime(Vec2 start, Vec2 end, TMXTiledMap* _tileMap);
	// 计算发射炮弹时计算炮弹的旋转方向
	static float getTurretRotation(Point monsterPoint, Point turret);

	static Vec2 & getBulletexX1(Vec2 monster, Vec2 turret, int num);
};

#endif