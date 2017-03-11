#ifndef __ProtectCastle__TurretData__
#define __ProtectCastle__TurretData__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

// TurretData类，用来存储关卡的炮塔信息
// "turrets": [{"name":"bollte.png","gold":100}]
class TurretData : public Ref
{
private:
	std::string _name; // 炮塔名称
	int _gold;  // 建造炮塔所需金币
	int _type;  // 炮塔的种类
	std::string _bulletName; // 炮塔发射的炮弹名称
public:
	// 静态的create函数
	static TurretData* create();

	// setter和getter函数
	void setName(std::string name);

	std::string getName();

	void setType(int type);

	int getType();

	void setGold(int gold);

	int getGold();

	void setBulletName(std::string bullet);

	std::string getBulletName();
};

#endif