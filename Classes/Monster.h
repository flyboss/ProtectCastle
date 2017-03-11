#ifndef __ProtectCastle__Monster__
#define __ProtectCastle__Monster__

#include <iostream>

#include "MonsterBase.h"
using namespace ui;

// ������
class Monster :public MonsterBase
{
private:
	int _lifeValue;  // ���������ֵ
	LoadingBar* _HP; // ����Ѫ��
	double _HPInterval; // ����Ѫ���ĸ�����
public:
	Monster() {};
	~Monster() {};
	// ��̬��create����
	static Monster* create(const std::string& filename);

	static Monster* createWithSpriteFrameName(const std::string& filename);

	// setter��getter����
	void setLifeValue(int lifeValue);

	int getLifeValue();

	void setHP(LoadingBar* HP);

	LoadingBar* getHP();

	void setHPInterval(double HPInterval);

	float getHpInterval();
	void getInjured();

};

#endif