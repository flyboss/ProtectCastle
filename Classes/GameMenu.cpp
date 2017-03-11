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
// 游戏菜单类
Scene* GameMenu::createScene()
{
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();

	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = GameMenu::create();

	// 将GameMenu层作为子节点添加到场景
	scene->addChild(layer);

	// 返回场景对象
	return scene;
}

//  "init" 函数中初始化实例
bool GameMenu::init()
{
	// 调用父类的init函数
	if (!Layer::init())
	{
		return false;
	}

	// 获得设备支持的可见OpenGL视图大小（即窗口大小）。
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 获得可见OpenGL视图的起源点（默认x=0，y=0）。
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 将精灵表单添加到缓存
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCESPLIST);
	// 预加载背景音乐和音效加载到
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE1);
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE2);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_FILE);

	// 添加一个关闭菜单，点击可以退出程序
	// 创建一个关闭图标（MenuItemImage），该对象会自动管理内存的释放
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameMenu::menuCloseCallback, this));
	// 设置关闭图标在屏幕的位置
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// 创建一个菜单（Menu）
	auto menu = Menu::create(closeItem, NULL);
	// 设置菜单项在屏幕的位置
	menu->setPosition(Vec2::ZERO);
	// 将菜单项添加到当前层的子节点
	this->addChild(menu, 1);

	Sprite*  bg = Sprite::create("sky.png");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(bg, 0);


	// “开始游戏”按钮
	auto start_button = Button::create("btn-start.png");
	start_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 90));
	// “开始游戏”按钮添加触摸监听
	start_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			// 左右滑动剧场效果
			auto transition = TransitionSplitRows::create(2.0, Game::createSceneWithLevel(1));
			// push游戏场景
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
			// 左右滑动剧场效果
			auto transition = TransitionSplitRows::create(2.0, Game::createSceneWithLevel(level));
			// push游戏场景
			Director::getInstance()->pushScene(transition);
		}
	});


	this->addChild(continue_button, 1);

	// “选择关卡”按钮
	auto select_button = Button::create("btn-select.png");
	select_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 90));
	// “选择关卡”按钮添加触摸监听
	select_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			// 右边滑入剧场效果
			auto transition = TransitionSlideInR::create(2.0, LevelManager::createScene());
			// push游戏场景
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
	// 程序执行结束,释放运行场景。
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}