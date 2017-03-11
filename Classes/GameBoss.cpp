#include "GameBoss.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

static int monsterCount = 1; // 每一波出现多少怪物
static int _currentLevel = 1;  // 当前关卡

Scene* GameBoss::createSceneWithLevel(int selectLevel)
{
	// 获得关卡数
	_currentLevel = selectLevel;
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();
	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = GameBoss::create();
	// 将Game层作为子节点添加到场景
	scene->addChild(layer);
	// 返回场景对象
	return scene;
}

bool GameBoss::init()
{
	if (!initialize(_currentLevel))
	{
		return false;
	}
	_castle->setVisible(false);
	// 计时器
	_time = 60;
	_currNumLabel = Label::createWithSystemFont(StringUtils::format("%d seconds left~", _time), "Arial", 32);
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

void GameBoss::update(float delta){
	_count++;
	updateMonster(delta);
	detectionTurret(delta);
	collisionDetection(delta);
	updateHUD(delta);
	updateLable(delta);
}

void GameBoss::updateMonster(float delta)
{
	// _isFinish标记为false表示可以出现怪物
	if (!_isFinish) {
		// _delivery变量为取模基数，通过设置该变量值，可以控制怪物出现频率
		if (_count % _delivery == 0) {
			// 当前出现怪物数量加1
			_currentCount++;
			// 如果当前出现怪物数量大于每一波出现多少怪物，表示一波结束
			if (_currentCount > monsterCount) {
				// 关卡怪物总波数减1
				_number -= 1;
				// 当关卡怪物总波数为0
				if (_number == 0) {
					// 设置标记，表示关卡所有怪物全部出现完毕
					_isFinish = true;
				}
				else{
					// 重新开始一波怪物
					_currentCount = 0; // 当前出现怪物数量归零
					_currNum++; // 当前出现怪物波数加1
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
	else
	{
		// _isFinish标记为true表示关卡所有怪物已经出现完毕
		if (_monsterVector.size() <= 0) {
			// 游戏胜利
			gameOver(1);
		}
	}
}

void GameBoss::monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector, Monster* monster){
	// 定义动作集合，用来保存多个moveTo动作
	Vector<FiniteTimeAction*> actionVector;
	// 循环路径点集合，创建多个MoveTo动作，玩家将执行多个MoveTo动作完成移动
	for (int i = 0; i < pathVector.size(); i++) {
		// 获得需要移动的每一个点
		PointDelegate* pd = pathVector.at(i);
		Vec2 glPoint = Vec2(pd->getX(), pd->getY());
		// 创建MoveTo一个动作，让精灵对象移动到指定的位置
		MoveTo* moveTo = MoveTo::create(0.2f, glPoint);
		// 将MoveTo动作添加到临时数组
		actionVector.pushBack(moveTo);
	}
	// 截取怪物名称
	std::string monsterName = monster->getName();
	size_t iPos = monsterName.find(".png");
	monsterName = monsterName.substr(0, iPos);
	// 调用getAnimateByName函数获得玩家精灵移动动画
	auto animate = getAnimateByName(monsterName, 0.5f, 5);
	// 创建一个动作，重复执行Animate动画
	auto repeatanimate = RepeatForever::create(animate);
	// 玩家精灵重复执行动画动作
	monster->runAction(repeatanimate);
	// 创建回调动作，当MoveTo动作完成后精灵恢复最初站立状态
	auto callfunc = CallFunc::create([=]{
		// 停止动画
		monster->stopAction(repeatanimate);
	});
	actionVector.pushBack(callfunc);
	// 按顺序执行动作集合中的动作
	auto sequence = Sequence::create(actionVector);
	auto repeatesequence = RepeatForever::create(sequence);
	// 执行一系列的动作
	monster->runAction(repeatesequence);
}

void GameBoss::updateLable(float delta)
{
	// 更新计时器
	if (_count % 60 == 0)
	{
		_time--;
		if (_time < 1)
		{
			gameOver(0);
		}
	}
	_currNumLabel->setString(StringUtils::format("%d seconds left~", _time));
}

void GameBoss::gameOver(int tag)
{

	// 停止游戏主循环
	this->unscheduleUpdate();
	// 停止所有怪物动作
	for (unsigned int i = 0; i < _monsterVector.size(); i++) {
		auto monster = _monsterVector.at(i);
		monster->stopAllActions();
	}
	// 删除屏幕所有炮弹
	for (unsigned int i = 0; i < _bulletVector.size(); i++) {
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

	// 你共击退了10波怪物!
	//	std::string messageString = "你共击退了";
	//	sscanf(messageString, "");
	auto messageLabel = Label::createWithSystemFont(StringUtils::format("You have repelled [%d]\n waves of monsters!", _currNum), "Arial", 38);
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

	// “再试一次”或"进入下一关"按钮
	auto play_button = Button::create();
	if (tag == 1){
		// 进入下一关
		play_button->loadTextures("btn-next.png", "btn-next.png");
		_currentLevel++;
	}
	else{
		// 再试一次
		//////////////////////////////////////////////////////////////////////////////////////messageLabel
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE2);
		play_button->loadTextures("btn-again.png", "btn-again.png");
	}
	play_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			if (_currentLevel > 3){
				auto transition = TransitionFade::create(2.0, GameEnd::createScene());
				Director::getInstance()->pushScene(transition);
			}
			else{
				// TransitionSplitRows 左右滑动剧场效果
				auto transition = TransitionSplitRows::create(2.0, GameBoss::createSceneWithLevel(_currentLevel));
				// push游戏场景
				Director::getInstance()->pushScene(transition);
			}
		}
	});
	play_button->setPosition(Vec2(_screenWidth / 2 + 100, _screenHeight / 2 - 80));
	this->addChild(play_button, 3);
}
