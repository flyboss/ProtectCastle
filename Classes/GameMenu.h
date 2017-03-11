#ifndef __ProtectCastle__GameMenuScene__
#define __ProtectCastle__GameMenuScene__

#include <iostream>
#include "cocos2d.h"
#include "LevelManager.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

// ��Ϸ�ľ�������������ֺ���Ч
#define RESOURCESPLIST "resources.plist"
#define MUSIC_FILE        "background.mp3"
#define EFFECT_FILE1        "Fat141.mp3"
#define EFFECT_FILE2       "Land232.mp3"

// ��Ϸ�˵���
class GameMenu : public cocos2d::Layer
{
public:
	/**
	* ��̬����������Scene����
	* ע�⣺��Cocos2d-x��û��Cocos2d-iPhone�еġ�id��,���鷵����ʵ����ָ��
	*/
	static cocos2d::Scene* createScene();
	/**
	* init��������ɳ�ʼ��������
	* ע�⣺init������Cocos2d-x�з���boolֵ�������Ƿ���Cocos2d-iPhone�еġ�id��
	*/
	virtual bool init();

	// ѡ�����ص�����
	void menuCloseCallback(cocos2d::Ref* pSender);

	/**
	* ʹ��CREATE_FUNC�괴����ǰ��Ķ��󣬷��صĶ��󽫻����Զ��ͷųع����ڴ���ͷ�
	*/
	CREATE_FUNC(GameMenu);
};

#endif