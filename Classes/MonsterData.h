#ifndef __ProtectCastle__MonsterData__
#define __ProtectCastle__MonsterData__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

// MonsterData�࣬�����洢�ؿ��Ĺ�����Ϣ
// "monsters": [{"name":"pest.png","lifeValue":2,"gold":15}]
class MonsterData : public Ref
{
private:
	std::string _name; // ��������
	int _lifeValue;  // ���������ֵ
	int _gold; // ��������õĽ��
public:
	// ��̬��create����
	static MonsterData* create();

	// setter��getter����
	void setName(std::string name);

	std::string getName();

	void setLifeValue(int lifeValue);

	int getLifeValue();

	int getMonsterType();

	void setGold(int gold);

	int getGold();
};


#endif