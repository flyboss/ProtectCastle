#include "Monster1.h"



Monster1::Monster1()
{
}


Monster1::~Monster1()
{

}

Monster1* Monster1::create(const std::string& filename)
{
	Monster1 *sprite = new Monster1();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Monster1* Monster1::createWithSpriteFrameName(const std::string& filename)
{
	Monster1 *sprite = new Monster1();
	if (sprite && sprite->initWithSpriteFrameName(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Monster1::getInjured()
{
	setLifeValue(getLifeValue() - 1);
}