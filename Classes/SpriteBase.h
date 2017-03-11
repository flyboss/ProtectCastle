#ifndef __ProtectCastle__SpriteBase__
#define __ProtectCastle__SpriteBase__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
// ��չ�����࣬����ͨ�õ�����
class SpriteBase : public cocos2d::Sprite
{
private:
	std::string _name; // ���������
	int _gold; // ����Ľ��ֵ
public:
	// ��̬��create����
	static SpriteBase* create(const std::string& filename);

	static SpriteBase* createWithSpriteFrameName(const std::string& filename);

	// setter��getter����
	void setName(std::string name);

	std::string getName();

	void setGold(int gold);

	int getGold();
};

#endif