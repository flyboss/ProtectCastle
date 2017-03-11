#include "Monster2.h"
#include <time.h>


Monster2::Monster2()
{
}


Monster2::~Monster2()
{

}

Monster2* Monster2::create(const std::string& filename)
{
	Monster2 *sprite = new Monster2();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Monster2* Monster2::createWithSpriteFrameName(const std::string& filename)
{
	Monster2 *sprite = new Monster2();
	if (sprite && sprite->initWithSpriteFrameName(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Monster2::getInjured()
{
	srand((int)time(0));
	int x = 1 + (int)(100.0*rand() / (RAND_MAX + 1.0));
	if (x>dodge)
	{
		setLifeValue(getLifeValue()+armor - 1);
	}
}