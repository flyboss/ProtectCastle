//
//  LevelData.cpp
//  ProtectPrincess
//
//  
//
//

#include "LevelData.h"

LevelData* LevelData::create(unsigned int number, std::string bg, std::string data)
{
	LevelData *sprite = new LevelData();
	if (sprite)
	{
		sprite->autorelease();
		sprite->setNumber(number);
		sprite->setBg(bg);
		sprite->setData(data);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void LevelData::setNumber(unsigned int number)
{
	_number = number;
}

unsigned int LevelData::getNumber()
{
	return _number;
}

void LevelData::setBg(std::string bg)
{
	_bg = bg;
}

std::string LevelData::getBg()
{
	return _bg;
}

void LevelData::setData(std::string data)
{
	_data = data;
}

std::string LevelData::getData()
{
	return _data;
}