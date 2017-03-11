#ifndef __ProtectCastle__TurretData__
#define __ProtectCastle__TurretData__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

// TurretData�࣬�����洢�ؿ���������Ϣ
// "turrets": [{"name":"bollte.png","gold":100}]
class TurretData : public Ref
{
private:
	std::string _name; // ��������
	int _gold;  // ��������������
	int _type;  // ����������
	std::string _bulletName; // ����������ڵ�����
public:
	// ��̬��create����
	static TurretData* create();

	// setter��getter����
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