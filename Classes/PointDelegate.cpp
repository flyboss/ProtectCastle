//
//  PointDelegate.cpp
//  ProtectPrincess
//
// 
//
//

#include "PointDelegate.h"

PointDelegate* PointDelegate::create(double x, double y)
{
	PointDelegate* p = new PointDelegate();
	if (p && p->initPoint(x, y))
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;

}

bool PointDelegate::initPoint(double x, double y)
{
	_x = x;
	_y = y;
	return true;
}

void PointDelegate::setX(double x)
{
	_x = x;
}

double PointDelegate::getX()
{
	return _x;
}

void PointDelegate::setY(double y)
{
	_y = y;
}

double PointDelegate::getY()
{
	return _y;
}