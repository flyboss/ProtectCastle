#ifndef __ProtectCastle__Turret__
#define __ProtectCastle__Turret__

#include "cocos2d.h"
#include "SpriteBase.h"
#include "Bullet.h"
#include "Monster.h"
#include "LaunchAlgorithm.h"
#include "SingleAlgorithm.h"
#include "TripleAlgorithm.h"
#include "MultipleAlgorithm.h"
// 炮塔类
class Turret : public SpriteBase
{
private:
	int _mtype = 0;
	std::string _bulletName; // 炮塔发射的炮弹名称
	bool _select = false; // 炮塔是否被安放
	Bullet* _bullet = nullptr; // 炮塔关联的炮弹对象
	int _time = 0;//炮塔发射时间
	LaunchAlgorithm * _LAL;
public:
	Turret() {}
	// 静态的create函数
	static Turret* create(const std::string& filename);

	static Turret* createWithSpriteFrameName(const std::string& filename);

	// 检测是否在圆攻击范围内
	static bool checkPointInCircle(Vec2 monsterPoint, Vec2 turretPoint, int area);
	// setter和getter函数
	void setBulletName(std::string bulletName);

	std::string getBulletName();

	void setSelect(bool select);

	bool isSelect();

	void setBullet(Bullet* bullet);

	Bullet* getBullet();

	void setTime(int time);

	int getTime();

	void setType(int type);

	int getType();
	
	void Launch(TMXTiledMap* _tileMap, Monster* monster, Vector<Bullet*> &_bulletVector);
	
	void setALA();

	LaunchAlgorithm* getALA();
};

#endif