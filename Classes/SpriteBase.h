#ifndef __ProtectCastle__SpriteBase__
#define __ProtectCastle__SpriteBase__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
// 扩展精灵类，增加通用的属性
class SpriteBase : public cocos2d::Sprite
{
private:
	std::string _name; // 精灵的名称
	int _gold; // 精灵的金币值
public:
	// 静态的create函数
	static SpriteBase* create(const std::string& filename);

	static SpriteBase* createWithSpriteFrameName(const std::string& filename);

	// setter和getter函数
	void setName(std::string name);

	std::string getName();

	void setGold(int gold);

	int getGold();
};

#endif