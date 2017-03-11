#ifndef __ProtectCastle__PointDelegate__
#define __ProtectCastle__PointDelegate__

#include <iostream>
#include "cocos2d.h"

// Point的代理类，继承cocos2d::Ref类
class PointDelegate : public cocos2d::Ref
{
private:
	double _x;
	double _y;
public:
	static PointDelegate* create(double x, double y);

	bool initPoint(double x, double y);

	void setX(double x);

	double getX();

	void setY(double y);

	double getY();

};

#endif