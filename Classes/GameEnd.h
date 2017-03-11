#ifndef __ProtectCastle__GameEndScene__
#define __ProtectCastle__GameEndScene__

#include <iostream>
#include "cocos2d.h"
#include "LevelManager.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


// 游戏菜单类
class GameEnd : public cocos2d::Layer
{
public:
	/**
	* 静态函数，创建Scene场景
	* 注意：在Cocos2d-x中没有Cocos2d-iPhone中的“id”,建议返回类实例的指针
	*/
	static cocos2d::Scene* createScene();
	/**
	* init函数，完成初始化操作。
	* 注意：init函数在Cocos2d-x中返回bool值，而不是返回Cocos2d-iPhone中的“id”
	*/
	virtual bool init();

	// 选择器回调函数
	void menuCloseCallback(cocos2d::Ref* pSender);

	/**
	* 使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	*/
	CREATE_FUNC(GameEnd);
};

#endif