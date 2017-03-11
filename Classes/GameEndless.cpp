#include "GameEndless.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

static int monsterCount = 1; // 每一波出现多少怪物
static int _currentLevel = 1;  // 当前关卡

Scene* GameEndless::createSceneWithLevel(int selectLevel)
{
	// 获得关卡数
	_currentLevel = selectLevel;
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();
	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = GameEndless::create();
	// 将Game层作为子节点添加到场景
	scene->addChild(layer);
	// 返回场景对象
	return scene;
}

bool GameEndless::init()
{
	if (!initialize(_currentLevel))
	{
		return false;
	}
	_sumNum = 1;
	// 共多少波怪物
	_currNumLabel = Label::createWithSystemFont(StringUtils::format("This is the %d wave of monsters", _sumNum), "Arial", 32);
	_currNumLabel->setColor(Color3B::BLUE);
	_currNumLabel->setPosition(_screenWidth*0.5, _screenHeight*0.96);
	_tileMap->addChild(_currNumLabel, 2);
	//开始游戏主循环
	auto countdown = Sequence::create(
		DelayTime::create(4),
		CallFunc::create([=]{
		scheduleUpdate();
	}),
		NULL);
	this->runAction(countdown);
	return true;
}

void GameEndless::update(float delta)
{
	_count++;
	updateMonster(delta);
	detectionTurret(delta);
	collisionDetection(delta);
	detectionPrincess(delta);
	updateHUD(delta);
	updateLable(delta);
}

void GameEndless::updateMonster(float delta){
	// _isFinish标记为false表示可以出现怪物
	if (!_isFinish) {
		if (_isThisFinish)
		{
			// _delivery变量为取模基数，通过设置该变量值，可以控制怪物出现频率
			if (_count % _delivery == 0) {
				// 当前出现怪物数量加1
				_currentCount++;
				if (_currentCount == monsterCount)
				{
					_isThisFinish = false;
				}
				// 如果当前出现怪物数量大于每一波出现多少怪物，表示一波结束
				if (_currentCount > monsterCount) {
					// 关卡怪物总波数减1
					_number -= 1;
					// 当关卡怪物总波数为0
					if (_number == 0) {
						// 设置标记，表示关卡所有怪物全部出现完毕
						_isFinish = true;
						_sumNum++;
					}
					else{
						// 重新开始一波怪物
						_currentCount = 0; // 当前出现怪物数量归零
						//						_delivery -= 5; // 改变取模基数，则怪物出现速度加快
						_currNum++; // 当前出现怪物波数加1
						_sumNum++;
						_monsterDatas = _monsterAllDatas.at(_currNum - 1);
						monsterCount = _monsterDatas->getCount();
						_delivery = _monsterDatas->getDelivery();
					}
				}
				else{
					// 调用TMXTiledMap的getObjectGroup函数获取对象层
					auto pestObject = _tileMap->getObjectGroup("monsterObject");
					// 根据对象名称获取对象的信息
					ValueMap pestValueMap = pestObject->getObject("monster");
					// 获取地图中设置的player对象的x和y值
					int pestX = pestValueMap.at("x").asInt();
					int pestY = pestValueMap.at("y").asInt();
					// 一个关卡可能出现多种怪物，可以在json文件中设置
					int random = rand() % _monsterDatas->size();
					auto monsterData = _monsterDatas->at(random);
					// 根据怪物名称创建怪物
					auto monster = Monster::create(monsterData->getName());
					_aliveCount++;

					// 设置怪物名称、生命值、消灭怪物的金币值和怪物出现坐标
					monster->setName(monsterData->getName());
					monster->setLifeValue(monsterData->getLifeValue());
					monster->setGold(monsterData->getGold());
					monster->setScale(0.7);
					monster->setPosition(pestX, pestY);

					// 创建进度条
					auto loadingBar = LoadingBar::create("planeHP.png");
					// 设置进度条方向
					loadingBar->setDirection(LoadingBar::Direction::LEFT);
					// 设置进度条大小
					loadingBar->setScale(0.06f, 0.1f);
					// 设置进度条百分比
					loadingBar->setPercent(100);
					// 将进度条设置为怪物血条
					monster->setHP(loadingBar);
					// 添加怪物血条
					_tileMap->addChild(monster->getHP(), 2);
					// 计算血条更新量
					monster->setHPInterval(100 / monster->getLifeValue());
					// 将怪物添为地图子节点
					_tileMap->addChild(monster, 2);
					// 将怪物添加到已出现怪物数组
					_monsterVector.pushBack(monster);
					// 怪物根据路径点移动
					monsterMoveWithWayPoints(_pathPoints, monster);
				}
			}
		}
	}
	else{
		// _isFinish标记为true表示关卡所有怪物已经出现完毕
		if (_monsterVector.size() <= 0) {
			// 游戏胜利
			for (int i = 0; i < _monsterAllDatas.size(); i++)
			{
				_monsterAllDatas.at(i)->upgrade();
			}
			_currentCount = 0; // 当前出现怪物数量归零
			_currNum = 1; // 当前出现怪物波数设为1
			_monsterDatas = _monsterAllDatas.at(_currNum - 1);
			monsterCount = _monsterDatas->getCount();
			_delivery = _monsterDatas->getDelivery();
			_number = _monsterAllDatas.size();
			_isFinish = false;
		}
	}
}

void GameEndless::updateLable(float delta)
{
	// 更新怪物波数量
	_currNumLabel->setString(StringUtils::format("This is the %d wave of monsters", _sumNum));
}

void GameEndless::gameOver(int tag)
{
	// 停止游戏主循环
	this->unscheduleUpdate();
	// 停止所有怪物动作
	for (unsigned int i = 0; i < _monsterVector.size(); i++)
	{
		auto monster = _monsterVector.at(i);
		monster->stopAllActions();
	}
	// 删除屏幕所有炮弹
	for (unsigned int i = 0; i < _bulletVector.size(); i++)
	{
		auto bullet = _bulletVector.at(i);
		bullet->stopAllActions();
		_tileMap->removeChild(bullet);
	}
	_bulletVector.clear();
	// 选择屏幕块
	auto temp = Sprite::createWithSpriteFrameName("block.png");
	temp->setPosition(_screenWidth / 2, _screenHeight / 2);
	temp->setColor(Color3B::GRAY);
	_tileMap->addChild(temp, 2);
	// 停止背景音乐
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(MUSIC_FILE);
	auto messageLabel = Label::createWithSystemFont(StringUtils::format("You have repelled [%d]\n waves of monsters!", _sumNum - 1), "Arial", 38);
	messageLabel->setColor(Color3B::WHITE);
	messageLabel->setPosition(_screenWidth / 2, _screenHeight / 2 + 50);
	_tileMap->addChild(messageLabel, 4);
	// “选择关卡”按钮
	auto select_button = Button::create("btn-select.png");
	select_button->setPosition(Vec2(_screenWidth / 2 - 100, _screenHeight / 2 - 80));
	select_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			// 右边滑入剧场效果
			auto transition = TransitionSlideInR::create(2.0, LevelManager::createScene());
			// push游戏场景
			Director::getInstance()->pushScene(transition);
		}
	});
	this->addChild(select_button, 3);

	// “再试一次”按钮
	auto play_button = Button::create();
	// 播放游戏失败音效
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE2);
	play_button->loadTextures("btn-again.png", "btn-again.png");

	play_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			// TransitionSplitRows 左右滑动剧场效果
			auto transition = TransitionSplitRows::create(2.0, GameEndless::createSceneWithLevel(_currentLevel));
			// push游戏场景
			Director::getInstance()->pushScene(transition);
		}
	});
	play_button->setPosition(Vec2(_screenWidth / 2 + 100, _screenHeight / 2 - 80));
	this->addChild(play_button, 3);
}
