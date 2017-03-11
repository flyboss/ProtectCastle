//
//  TurretData.cpp
//  ProtectPrincess
//
//  
//
//

#include "TurretData.h"

TurretData* TurretData::create()
{
	TurretData *sprite = new TurretData();
	if (sprite)
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void TurretData::setName(std::string name)
{
	_name = name;
}

void TurretData::setType(int type)
{
	_type = type;
}

int TurretData::getType()
{
	return _type;
}

void TurretData::setGold(int gold)
{
	_gold = gold;
}

int TurretData::getGold()
{
	return _gold;
}

void TurretData::setBulletName(std::string bullet)
{
	_bulletName = bullet;
}

std::string TurretData::getBulletName()
{
	return _bulletName;
}

std::string TurretData::getName()
{
	return _name;
}