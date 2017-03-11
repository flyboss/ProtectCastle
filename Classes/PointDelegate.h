#ifndef __ProtectCastle__PointDelegate__
#define __ProtectCastle__PointDelegate__

#include <iostream>
#include "cocos2d.h"

// Point�Ĵ����࣬�̳�cocos2d::Ref��
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