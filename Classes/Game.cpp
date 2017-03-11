#include "Game.h"
#include "ui/CocosGUI.h"
#include <fstream>
#include <cstdlib>

USING_NS_CC;
using namespace ui;
using namespace std;
static int monsterCount = 1; // 每一波出现多少怪物
static int _currentLevel = 1;  // 当前关卡

Scene* Game::createSceneWithLevel(int selectLevel)
{
	// 获得关卡数
	_currentLevel = selectLevel;
	//把关卡信息存入文件以作为备忘录
	ofstream  outfile;
	outfile.open("level.txt", ios::out | ios::trunc);
	if (outfile.is_open()) {
		outfile << _currentLevel;
		outfile.close();

	}

	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();
	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = Game::create();
	// 将Game层作为子节点添加到场景
	scene->addChild(layer);
	// 返回场景对象
	return scene;
}

bool Game::init()
{
	if (!initialize(_currentLevel))
	{
		return false;
	}
	//开始游戏主循环
	// 显示出现了多少波怪物
	_currNumLabel = Label::createWithSystemFont("0", "Arial", 32);
	_currNumLabel->setColor(Color3B::RED);
	_currNumLabel->setPosition(_screenWidth*0.375, _screenHeight*0.96);
	_tileMap->addChild(_currNumLabel, 2);
	// 共多少波怪物
	_numberLabel = Label::createWithSystemFont(StringUtils::format("/%d waves of monsters", _number), "Arial", 32);
	_numberLabel->setColor(Color3B::BLUE);
	_numberLabel->setPosition(_screenWidth*0.55, _screenHeight*0.96);
	_tileMap->addChild(_numberLabel, 2);

	auto countdown = Sequence::create(
		DelayTime::create(4), 
		CallFunc::create([=]{
		scheduleUpdate();
	}),
		NULL);
	this->runAction(countdown);
	return true;
}

bool Game::initialize(int currentLevel)
{
	if (!Layer::init())
	{
		return false;
	}
	
	_currentLevel = currentLevel;

	// 获得设备支持的可见OpenGL视图大小（即窗口大小）。
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;

	/*************读取关卡数据开始**********************/
	// rapidjson对象
	rapidjson::Document document;
	// 关卡数据使用json，Cocos2d-X 3.0中自带了rapidjson，用于解析Json。
	// 根据传递的关卡值解析对应的关卡数据文件
	std::string filePath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("%d.data", _currentLevel));
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(filePath);
	document.Parse<0>(contentStr.c_str());

	// 读取地图文件名称
	_tileFile = document["tileFile"].GetString();
	// 读取怪物波数
	_number = document["number"].GetInt();
	// 出现怪物取模基数
	_delivery = 40;
	// 当前出现怪物数量
	_currentCount = 0;
	_aliveCount = 0;
	_isThisFinish = true;
	// 当前怪物波数
	_currNum = 1;
	// 初始化金币数量
	_goldValue = 200;

	// 调用TMXTiledMap的函数读取TileMap.tmx文件创建一个瓦片地图对象
	_tileMap = TMXTiledMap::create(_tileFile);
	// 把地图的锚点和位置都设置为原点，这样可以使地图的左下角与屏幕的左下角对齐
	_tileMap->setAnchorPoint(Vec2::ZERO);
	_tileMap->setPosition(Vec2::ZERO);
	// 将地图添加为当前层的子节点
	this->addChild(_tileMap, 1);

	// 获取障碍层，并设置障碍层为不可见
	_collidable = _tileMap->getLayer("collidable");
	_collidable->setVisible(true);

	// 获得关卡设定的怪物
    rapidjson::Value& monstersArray = document["monsters"];
    MonsterContainer::loadMonster(*this,  monstersArray);
	monsterCount = _monsterDatas->getCount();

	// 获得关卡设定的炮塔
	const rapidjson::Value& turretArray = document["turrets"];
	for (int i = 0; i < turretArray.Size(); i++)
	{
		// 获得每一种炮塔的name、gold和bulletName数据
		std::string name = turretArray[i]["name"].GetString();
		int gold = turretArray[i]["gold"].GetInt();
		std::string bulletName = turretArray[i]["bulletName"].GetString();
		int type = turretArray[i]["type"].GetInt();
		// 将获得的name、gold和bulletName数据存储到自定义TurretData对象当中
		auto turretData = TurretData::create();
		turretData->setName(name);
		turretData->setGold(gold);
		turretData->setType(type);
		turretData->setBulletName(bulletName);
		// 将TurretData存储到关卡炮塔数据的集合当中
		_turretDatas.pushBack(turretData);
	}
	// 获得怪物经过的路径
	const rapidjson::Value& pathArray = document["path"];
	for (int i = 0; i < pathArray.Size(); i++){
		// 获得每个怪物经过路径的x和y值
		int x = pathArray[i]["x"].GetInt();
		int y = pathArray[i]["y"].GetInt();
		// 根据x和y值创建一个坐标，该坐标是一个地图坐标
		Vec2 tilePoint = Vec2(x, y);
		// 将地图坐标转成屏幕坐标
		Vec2 locationPoint = Coordinate::locationForTilePos(tilePoint,_tileMap);
		// 由于Point不继承Ref，Vector不能存储，所以设计了PointDelegate类代理存储数据
		auto pointDelegate = PointDelegate::create(locationPoint.x, locationPoint.y);
		// 将每一个屏幕坐标存储到路径集合当中
		_pathPoints.pushBack(pointDelegate);
	}
	_castle = Princess::getInstance(_tileMap);
	/*************读取关卡数据结束**********************/


	//“暂停”按钮
	auto pause_button = Button::create("pause.png");
	pause_button->setPosition(Vec2(40, visibleSize.height / 2 + 240));
	// “暂停”按钮添加触摸监听
	pause_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			// 暂停
			this->pause();
			for (unsigned int i = 0; i < _monsterVector.size(); i++) {
				auto monster = _monsterVector.at(i);
				monster->pause();
			}

		}
	});
	this->addChild(pause_button, 1);

	//“恢复”按钮
	auto start_button = Button::create("play.png");
	start_button->setPosition(Vec2(80, visibleSize.height / 2 + 240));
	start_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			//恢复
			this->resume();
			for (unsigned int i = 0; i < _monsterVector.size(); i++) {
				auto monster = _monsterVector.at(i);
				monster->resume();
			}
		}
	});
	this->addChild(start_button, 1);

	// “选择关卡”按钮
	auto select_button = Button::create("btn-select.png");
	select_button->setPosition(Vec2(60, visibleSize.height / 2 + 175));
	// “选择关卡”按钮添加触摸监听
	select_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			// 右边滑入剧场效果
			//auto transition = TransitionSlideInR::create(2.0, LevelManager::createScene());//LevelManager::createScene());!!!
			// push游戏场景
			SimpleAudioEngine::getInstance()->stopBackgroundMusic(MUSIC_FILE);
			Director::getInstance()->pushScene(LevelManager::createScene());
			//Director::getInstance()->pushScene(transition);
		}
	});
	this->addChild(select_button, 1);

	/*************设置屏幕上方显示的游戏数据开始**********************/
	// 左上角玩家金币数量
	auto gold = Sprite::createWithSpriteFrameName("gold.png");
	gold->setPosition(50, _screenHeight*0.96);
	_tileMap->addChild(gold, 2);

	_goldLabel = Label::createWithSystemFont("200", "Arial", 32);
	_goldLabel->setColor(Color3B::RED);
	_goldLabel->setPosition(100, _screenHeight*0.96);
	_tileMap->addChild(_goldLabel, 2);

	// 预先准备好可建造的炮塔 add.png
	addImage = Sprite::create("add.png");
	addImage->setScale(0.75);
	addImage->setVisible(false);
	_tileMap->addChild(addImage, 3);
	// 将可建造炮塔添加为addImage的子节点
	for (unsigned int i = 0; i<_turretDatas.size(); i++) {
		// 循环炮塔数据的集合获取每一个炮塔
		auto data = _turretDatas.at(i);
		// 创建炮塔
		auto turret = Turret::create(data->getName());
		turret->setGold(data->getGold());
		turret->setType(data->getType());
		turret->setScale(1.8);
		int rest = i % 2;
		int q = i / 2;
		turret->setPosition(60 * rest, (q + 1)*addImage->getContentSize().height*1.5 + 16);
		// 将炮塔添加为addImage的子节点
		addImage->addChild(turret);
	}

	// 创建事件监听器
	auto gameListener = EventListenerTouchOneByOne::create();
	// 响应触摸事件函数
	gameListener->onTouchBegan = [=](Touch* touch, Event* event){
		// OpenGL坐标
		Vec2 touchLocation = touch->getLocation();
		// 获得addImage所有子节点（即炮塔）
		Vector<Node*> turrets = addImage->getChildren();
		for (unsigned int i = 0; i<turrets.size(); i++) {
			// 获得每一个炮塔
			auto node = turrets.at(i);
			Rect rect = Rect(0, 0, node->getContentSize().width, node->getContentSize().height);
			Turret* turret = (Turret*)node;
			// 如果addImage显示，说明玩家是第二次触摸，选择建造炮塔
			if (addImage->isVisible())
			{
				// 如果不够金币建造炮塔，返回false
				if (turret->getGold() > _goldValue) {
					_isSelect = false;
					addImage->setVisible(false);
					return false;
				}
				else{
					// 如果可以建造炮塔，设置_isSelect=true
					if (rect.containsPoint(node->convertToNodeSpace(touchLocation))) {
						addImage->setTag(i);
						_isSelect = true;
						return true;
					}
				}
			}
			// 玩家是第一次触摸
			else
			{
				// 如果建造炮塔金币不够，设置炮塔透明度，表示不可建造
				if (turret->getGold() > _goldValue) {
					turret->setOpacity(100);
				}
			}
		}
		return true;
	};
	gameListener->onTouchEnded = [=](Touch *touch, Event *event){
		// OpenGL坐标
		Vec2 touchLocation = touch->getLocation();
		// 相对Node的坐标
		Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
		auto tempLocation = Coordinate::tileCoordForPosition(nodeLocation,_tileMap);
		tempLocation.x -= static_cast<int>(tempLocation.x) % 2 - 1;
		tempLocation.y -= static_cast<int>(tempLocation.y) % 2 - 1;
		auto addLocation = Coordinate::locationForTilePos(tempLocation, _tileMap);
		// 同一个位置上不能放置炮塔
		for (int i = 0; i < _turretVector.size(); i++) {
			auto temp = _turretVector.at(i);
			if (temp->getBoundingBox().containsPoint(addLocation)) {
				return;
			}
		}
		// _isSelect为true，用户是选择建造炮塔，false，用户选择建造炮塔的位置
		if (_isSelect) {
			_isSelect = false;
			// 创建一个炮塔对象
			TurretData *data = _turretDatas.at(addImage->getTag());
			Turret *turret = Turret::create(data->getName());
			turret->setScale(1.4);
			// 将炮塔设置在addImage的位置上
			turret->setPosition(addImage->getPosition());
			turret->setBulletName(data->getBulletName());
			turret->setType(data->getType());
			turret->setGold(data->getGold());
			_tileMap->addChild(turret, 1);
			// 隐藏addImage
			addImage->setVisible(false);
			// 将炮塔添加到炮塔集合
			_turretVector.pushBack(turret);
			// 消费金币
			_goldValue -= data->getGold();
		}
		else{
			// 如果准备建造炮塔的位置不是障碍物时
			if (!Coordinate::getCollidable(nodeLocation,_tileMap, _collidable)) {
				// addImage显示
				addImage->setVisible(true);
				// 设置addImage位置为触碰位置
				//				nodeLocation.x -= static_cast<int>(nodeLocation.x) % 60 - 20;
				//				nodeLocation.y -= static_cast<int>(nodeLocation.y) % 60 - 40;
				addImage->setPosition(addLocation);
			}
		}
	};


	// 添加场景优先事件监听器
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener, this);

	// 开始游戏，倒数3下
	Label* label1 = Label::createWithSystemFont("1", "Arial-BoldMT", 150);
	Label* label2 = Label::createWithSystemFont("2", "Arial-BoldMT", 150);
	Label* label3 = Label::createWithSystemFont("3", "Arial-BoldMT", 150);
	label1->setColor(Color3B::BLUE);
	label2->setColor(Color3B::BLUE);
	label3->setColor(Color3B::BLUE);
	label1->setPosition(_screenWidth / 2, _screenHeight / 2);
	label2->setPosition(_screenWidth / 2, _screenHeight / 2);
	label3->setPosition(_screenWidth / 2, _screenHeight / 2);
	// 设置Label不显示
	label1->setVisible(false);
	label2->setVisible(false);
	label3->setVisible(false);
	_tileMap->addChild(label1, 2);
	_tileMap->addChild(label2, 2);
	_tileMap->addChild(label3, 2);
	// 完成倒数的Sequence动作，3，2，1，每隔1秒显示1个数字，同时删除之前显示的数字
	auto countdown = Sequence::create(
		CallFunc::create([=]{
		label3->setVisible(true);
	}), DelayTime::create(2), CallFunc::create([=]{
		_tileMap->removeChild(label3);
	}), CallFunc::create([=]{
		label2->setVisible(true);
	}), DelayTime::create(1), CallFunc::create([=]{
		_tileMap->removeChild(label2);
	}), CallFunc::create([=]{
		label1->setVisible(true);
	}), DelayTime::create(1), CallFunc::create([=]{
		_tileMap->removeChild(label1);
	}), NULL);
	// 执行倒数动作
	this->runAction(countdown);

	// 播放背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_FILE, true);
	// 设置背景音乐大小
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);

	/*************设置屏幕上方显示的游戏数据结束**********************/

	return true;
}

// 游戏主循环
void Game::update(float delta)
{
	MonsterContainer::updateMonster(delta, *this, monsterCount);
	detectionTurret(delta);
	collisionDetection(delta);
	detectionPrincess(delta);
	updateHUD(delta);
	updateLable(delta);
	_count++;

}


// 获得动画动作函数
Animate* Game::getAnimateByName(std::string animName, float delay, int animNum)
{
	// 创建动画
	Animation* animation = Animation::create();
	// 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
	for (unsigned int i = 1; i <= animNum; i++){
		// 获取动画图片名称，例如pest1.png
		std::string frameName = animName;
		frameName.append(StringUtils::format("%d.png", i));
		// 将单张图片添加为精灵帧（即动画帧）
		animation->addSpriteFrameWithFile(frameName.c_str());
	}
	// 设置动画播放的属性
	animation->setDelayPerUnit(delay);
	// 让精灵对象在动画执行完后恢复到最初状态
	animation->setRestoreOriginalFrame(true);
	// 返回动画动作对象
	Animate* animate = Animate::create(animation);
	return animate;
}

void Game::updateHUD(float delta)
{
	// 更新金币数量
	_goldLabel->setString(StringUtils::format("%d", _goldValue));
	// 设置每一个怪物血条位置，血条跟随怪物移动
	for (unsigned int i = 0; i < _monsterVector.size(); i++) {
		auto monster = _monsterVector.at(i);
		// 如果怪物不为nullptr，并且怪物生命值不为0的时候
		if (monster != nullptr && monster->getLifeValue() > 0) {
			// 怪物精灵坐标
			Vec2 pos = monster->getPosition();
			// 设置怪物血条显示的坐标位置
			monster->getHP()->setPosition(Vec2(pos.x, pos.y + 40));
		}
	}
	for (unsigned i = 0; i < _turretDatas.size(); i++)
	{
		auto turret = (Turret*)addImage->getChildren().at(i);
		if (turret->getGold() <= _goldValue)
		{
			turret->setOpacity(255);
		}
	}
}

void Game::updateLable(float delta)
{
	// 更新怪物波数量
	_currNumLabel->setString(StringUtils::format("%d", _currNum));
}


// 怪物根据路径点移动
void Game::monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector, MonsterBase* monster){
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
	// 执行一系列的动作
	monster->runAction(sequence);
}

// 检测炮塔
void Game::detectionTurret(float delta)
{
	// 遍历炮塔集合
	for (unsigned int i = 0; i < _turretVector.size(); i++) {
		// 获得每一个炮塔
		auto turret = _turretVector.at(i);
		turret->setTime(turret->getTime() + 1);
		// 游戏设定炮塔一次只能发射一枚炮弹，如果炮塔没有关联炮弹或者炮弹已经发射完成
		if (turret->getTime() > 40)
		{
			// 遍历怪物集合
			turret->setTime(0);
			for (unsigned int j = 0; j < _monsterVector.size(); j++) {
				auto monster = _monsterVector.at(j);
				// 检测怪物是否在炮塔的攻击范围
				bool flag = Turret::checkPointInCircle(monster->getPosition(), turret->getPosition(), 200);
				if (flag==true)
				{
					turret->Launch(_tileMap,(Monster*)monster, _bulletVector);
					float cocosAngle = Bullet::getTurretRotation(monster->getPosition(), turret->getPosition());
					turret->runAction(RotateTo::create(0.05, cocosAngle));
					break;
				}
			}
		}
	}
}
// 检测炮弹和怪物的碰撞
void Game::collisionDetection(float delta)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 遍历怪物数组
	for (unsigned int i = 0; i < _monsterVector.size(); i++)
	{
		MonsterBase* monster = _monsterVector.at(i);
		// 遍历炮弹数组
		for (unsigned int j = 0; j < _bulletVector.size(); j++) {
			auto bullet = _bulletVector.at(j);
			// 如果怪物与炮弹发生了碰撞
			if (monster->getBoundingBox().intersectsRect(bullet->getBoundingBox())){
				// 设置怪物的生命值1

				monster->getInjured();
				// 更新怪物血条
				
				
				// 从炮弹集合中删除炮弹
					_bulletVector.eraseObject(bullet);
					// 修改炮弹发射标记
					bullet->setShoot(false);
					// 从地图上删除炮弹
					_tileMap->removeChild(bullet);
					break;
			}
			else
			{
				// 如果炮弹到达攻击坐标，而怪物已经走开，即炮弹没有打中怪物
				if (bullet->getNumberOfRunningActions() == 0){
					// 删除炮弹
					_bulletVector.eraseObject(bullet);
					// 修改炮弹发射标记
					bullet->setShoot(false);
					// 从地图上删除炮弹
					_tileMap->removeChild(bullet);
				}
				else if (bullet->getPosition().x <= 0 || bullet->getPosition().y <= 0
					|| bullet->getPosition().x >= visibleSize.width || (bullet->getPosition().y >= visibleSize.height))
				{
					_bulletVector.eraseObject(bullet);
					bullet->setShoot(false);
					_tileMap->removeChild(bullet);
				}
			}
		}
		if (monster->getHP() != nullptr) {
			monster->getHP()->setPercent(monster->getHpInterval()*monster->getLifeValue());
		}
		// 如果怪物生命值为0
		if (monster->getLifeValue() <= 0) {
			// 播放怪物被消灭的音效
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE1);
			// 从怪物集合中删除怪物
			_monsterVector.eraseObject(monster);
			// 金币增加
			_goldValue += monster->getGold();
			// 从地图上删除怪物
			_tileMap->removeChild(monster);
			_aliveCount--;
			if (_aliveCount == 0)
			{
				_isThisFinish = true;
			}
		}
	}
}

// 检测公主
void Game::detectionPrincess(float delta)
{
	// 遍历每一个怪物
	for (unsigned int j = 0; j < _monsterVector.size(); j++) {
		auto monster = _monsterVector.at(j);
		// 如果怪物与攻击公主，游戏失败
		if (monster->getBoundingBox().containsPoint(_castle->getPosition())){
			// 游戏失败
			gameOver(0);
		}
	}
}

// 游戏结束
void Game::gameOver(int tag)
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
		// 播放游戏失败音效
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE2);
		play_button->loadTextures("btn-again.png", "btn-again.png");
	}
	play_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			if (_currentLevel > 3){
				auto transition = TransitionFade::create(2.0, GameEnd::createScene());
				//Director::getInstance()->pushScene(GameEnd::createScene());
				Director::getInstance()->pushScene(transition);
			}
			else{
				// TransitionSplitRows 左右滑动剧场效果
				auto transition = TransitionSplitRows::create(2.0, Game::createSceneWithLevel(_currentLevel));
				// push游戏场景
				Director::getInstance()->pushScene(transition);
			}
		}
	});
	play_button->setPosition(Vec2(_screenWidth / 2 + 100, _screenHeight / 2 - 80));
	this->addChild(play_button, 3);
}