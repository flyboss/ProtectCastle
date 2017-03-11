#ifndef __ProtectCastle__LevelManager__
#define __ProtectCastle__LevelManager__

#include <iostream>
#include "cocos2d.h"
#include "json/document.h"
#include "LevelData.h"

// �ؿ�������
class LevelManager : public cocos2d::Layer
{
protected:
	int _selectLevelIndex; // ѡ��Ĺؿ�����
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
	CREATE_FUNC(LevelManager);
};

#endif