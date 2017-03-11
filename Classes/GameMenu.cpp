#include "GameMenu.h"
#include "Game.h"
#include "GameEndless.h"
#include "GameBoss.h"
#include "ui/CocosGUI.h"
#include <fstream>
#include <cstdlib>
USING_NS_CC;
using namespace ui;
using namespace std;
// ��Ϸ�˵���
Scene* GameMenu::createScene()
{
	// ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto scene = Scene::create();

	// ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto layer = GameMenu::create();

	// ��GameMenu����Ϊ�ӽڵ���ӵ�����
	scene->addChild(layer);

	// ���س�������
	return scene;
}

//  "init" �����г�ʼ��ʵ��
bool GameMenu::init()
{
	// ���ø����init����
	if (!Layer::init())
	{
		return false;
	}

	// ����豸֧�ֵĿɼ�OpenGL��ͼ��С�������ڴ�С����
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// ��ÿɼ�OpenGL��ͼ����Դ�㣨Ĭ��x=0��y=0����
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ���������ӵ�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCESPLIST);
	// Ԥ���ر������ֺ���Ч���ص�
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE1);
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE2);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_FILE);

	// ���һ���رղ˵�����������˳�����
	// ����һ���ر�ͼ�꣨MenuItemImage�����ö�����Զ������ڴ���ͷ�
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameMenu::menuCloseCallback, this));
	// ���ùر�ͼ������Ļ��λ��
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// ����һ���˵���Menu��
	auto menu = Menu::create(closeItem, NULL);
	// ���ò˵�������Ļ��λ��
	menu->setPosition(Vec2::ZERO);
	// ���˵�����ӵ���ǰ����ӽڵ�
	this->addChild(menu, 1);

	Sprite*  bg = Sprite::create("sky.png");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(bg, 0);


	// ����ʼ��Ϸ����ť
	auto start_button = Button::create("btn-start.png");
	start_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 90));
	// ����ʼ��Ϸ����ť��Ӵ�������
	start_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			// ���һ����糡Ч��
			auto transition = TransitionSplitRows::create(2.0, Game::createSceneWithLevel(1));
			// push��Ϸ����
			Director::getInstance()->pushScene(transition);
		}
	});
	this->addChild(start_button, 1);

	auto continue_button = Button::create("btn-continue.png");
	continue_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	continue_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
	{
		int level = 1;
		ifstream  infile("level.txt");
		if (!infile) {
			level = 1;
		}
		else {
			infile >> level;
			infile.close();
		}
		if (type == Widget::TouchEventType::ENDED)
		{
			// ���һ����糡Ч��
			auto transition = TransitionSplitRows::create(2.0, Game::createSceneWithLevel(level));
			// push��Ϸ����
			Director::getInstance()->pushScene(transition);
		}
	});


	this->addChild(continue_button, 1);

	// ��ѡ��ؿ�����ť
	auto select_button = Button::create("btn-select.png");
	select_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 90));
	// ��ѡ��ؿ�����ť��Ӵ�������
	select_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			// �ұ߻���糡Ч��
			auto transition = TransitionSlideInR::create(2.0, LevelManager::createScene());
			// push��Ϸ����
			Director::getInstance()->pushScene(transition);
		}
	});
	this->addChild(select_button, 1);

	return true;
}



void GameMenu::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	// ����ִ�н���,�ͷ����г�����
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}