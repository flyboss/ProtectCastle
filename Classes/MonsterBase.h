#ifndef __ProtectCastle__MonsterBase__
#define __ProtectCastle__MonsterBase__

#include <iostream>
#include "cocos2d.h"
#include "SpriteBase.h"
#include "ui/CocosGUI.h"
using namespace ui;
class MonsterBase: public SpriteBase
{
private:
	int _lifeValue;  // ���������ֵ
	LoadingBar* _HP; // ����Ѫ��
	double _HPInterval; // ����Ѫ���ĸ�����
	std::string _name; // ��������
	int _gold; // ��������õĽ��
public:
	MonsterBase();

	~MonsterBase();

	// ��̬��create����
	//virtual MonsterBase* create(const std::string& filename)=0 ;

	//virtual MonsterBase* createWithSpriteFrameName(const std::string& filename)=0 ;

	// setter��getter����
	void setName(std::string name);

	std::string getName();

	void setGold(int gold);

	int getGold();

	void setLifeValue(int lifeValue);

	int getLifeValue();

	void setHP(LoadingBar* HP);

	LoadingBar* getHP();

	void setHPInterval(double HPInterval);

	float getHpInterval();


	//�ӿ�
	virtual void getInjured() = 0;
};
#endif
