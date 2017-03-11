#include "GameEnd.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

// 游戏菜单类
Scene* GameEnd::createScene()
{
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();

	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = GameEnd::create();

	// 将GameMenu层作为子节点添加到场景
	scene->addChild(layer);

	// 返回场景对象
	return scene;
}

//  "init" 函数中初始化实例
bool GameEnd::init()
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
	   auto temp = Sprite::create("ground.png");
    // position the label on the center of the screen
    temp->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height /2));
    this->addChild(temp,0);

	// “选择关卡”按钮
	auto select_button = Button::create("btn-select.png");
	select_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 75));
	// “选择关卡”按钮添加触摸监听
	select_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			// 剧场效果
			auto transition = TransitionShrinkGrow::create(2.0, LevelManager::createScene());//LevelManager::createScene());!!!
			// push游戏场景
			Director::getInstance()->pushScene(transition);
		}
	});
	this->addChild(select_button, 1);
	return true;
}