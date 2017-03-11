#ifndef __ProtectCastle__Monster1__
#define __ProtectCastle__Monster1__


#include"MonsterBase.h"
class Monster1:public MonsterBase
{
private:
	int _lifeValue;  // ���������ֵ
	LoadingBar* _HP; // ����Ѫ��
	double _HPInterval; // ����Ѫ���ĸ�����
	std::string _name; // ��������
	int _gold; // ��������õĽ��
public:
	Monster1();
	~Monster1();
	static Monster1* create(const std::string& filename);
	static Monster1* createWithSpriteFrameName(const std::string& filename);
	void getInjured() ;
};

#endif