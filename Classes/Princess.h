#ifndef __ProtectPrincess__Princess__
#define __ProtectPrincess__Princess__

#include "SpriteBase.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace ui;
class Princess : public SpriteBase
{
private:
	Princess();
	~Princess() {};
	static Princess * _princess;												

public:
	static Princess* getInstance(TMXTiledMap* _tileMap);
};







#endif
