#ifndef __ProtectCastle__Monster2__
#define __ProtectCastle__Monster2__


#include"MonsterBase.h"
class Monster2 :public MonsterBase
{
private:
	int _lifeValue;  // ���������ֵ
	LoadingBar* _HP; // ����Ѫ��
	double _HPInterval; // ����Ѫ���ĸ�����
	std::string _name; // ��������
	int _gold; // ��������õĽ��
	int armor = 0;
	const int dodge=40;
public:
	Monster2();
	~Monster2();
	static Monster2* create(const std::string& filename);
	static Monster2* createWithSpriteFrameName(const std::string& filename);
	//����
	void getInjured();
};

#endif