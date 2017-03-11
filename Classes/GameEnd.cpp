#include "GameEnd.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

// ��Ϸ�˵���
Scene* GameEnd::createScene()
{
	// ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto scene = Scene::create();

	// ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto layer = GameEnd::create();

	// ��GameMenu����Ϊ�ӽڵ���ӵ�����
	scene->addChild(layer);

	// ���س�������
	return scene;
}

//  "init" �����г�ʼ��ʵ��
bool GameEnd::init()
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
	   auto temp = Sprite::create("ground.png");
    // position the label on the center of the screen
    temp->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height /2));
    this->addChild(temp,0);

	// ��ѡ��ؿ�����ť
	auto select_button = Button::create("btn-select.png");
	select_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 75));
	// ��ѡ��ؿ�����ť��Ӵ�������
	select_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			// �糡Ч��
			auto transition = TransitionShrinkGrow::create(2.0, LevelManager::createScene());//LevelManager::createScene());!!!
			// push��Ϸ����
			Director::getInstance()->pushScene(transition);
		}
	});
	this->addChild(select_button, 1);
	return true;
}