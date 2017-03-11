#include "Game.h"
#include "ui/CocosGUI.h"
#include <fstream>
#include <cstdlib>

USING_NS_CC;
using namespace ui;
using namespace std;
static int monsterCount = 1; // ÿһ�����ֶ��ٹ���
static int _currentLevel = 1;  // ��ǰ�ؿ�

Scene* Game::createSceneWithLevel(int selectLevel)
{
	// ��ùؿ���
	_currentLevel = selectLevel;
	//�ѹؿ���Ϣ�����ļ�����Ϊ����¼
	ofstream  outfile;
	outfile.open("level.txt", ios::out | ios::trunc);
	if (outfile.is_open()) {
		outfile << _currentLevel;
		outfile.close();

	}

	// ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto scene = Scene::create();
	// ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto layer = Game::create();
	// ��Game����Ϊ�ӽڵ���ӵ�����
	scene->addChild(layer);
	// ���س�������
	return scene;
}

bool Game::init()
{
	if (!initialize(_currentLevel))
	{
		return false;
	}
	//��ʼ��Ϸ��ѭ��
	// ��ʾ�����˶��ٲ�����
	_currNumLabel = Label::createWithSystemFont("0", "Arial", 32);
	_currNumLabel->setColor(Color3B::RED);
	_currNumLabel->setPosition(_screenWidth*0.375, _screenHeight*0.96);
	_tileMap->addChild(_currNumLabel, 2);
	// �����ٲ�����
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

	// ����豸֧�ֵĿɼ�OpenGL��ͼ��С�������ڴ�С����
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;

	/*************��ȡ�ؿ����ݿ�ʼ**********************/
	// rapidjson����
	rapidjson::Document document;
	// �ؿ�����ʹ��json��Cocos2d-X 3.0���Դ���rapidjson�����ڽ���Json��
	// ���ݴ��ݵĹؿ�ֵ������Ӧ�Ĺؿ������ļ�
	std::string filePath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("%d.data", _currentLevel));
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(filePath);
	document.Parse<0>(contentStr.c_str());

	// ��ȡ��ͼ�ļ�����
	_tileFile = document["tileFile"].GetString();
	// ��ȡ���ﲨ��
	_number = document["number"].GetInt();
	// ���ֹ���ȡģ����
	_delivery = 40;
	// ��ǰ���ֹ�������
	_currentCount = 0;
	_aliveCount = 0;
	_isThisFinish = true;
	// ��ǰ���ﲨ��
	_currNum = 1;
	// ��ʼ���������
	_goldValue = 200;

	// ����TMXTiledMap�ĺ�����ȡTileMap.tmx�ļ�����һ����Ƭ��ͼ����
	_tileMap = TMXTiledMap::create(_tileFile);
	// �ѵ�ͼ��ê���λ�ö�����Ϊԭ�㣬��������ʹ��ͼ�����½�����Ļ�����½Ƕ���
	_tileMap->setAnchorPoint(Vec2::ZERO);
	_tileMap->setPosition(Vec2::ZERO);
	// ����ͼ���Ϊ��ǰ����ӽڵ�
	this->addChild(_tileMap, 1);

	// ��ȡ�ϰ��㣬�������ϰ���Ϊ���ɼ�
	_collidable = _tileMap->getLayer("collidable");
	_collidable->setVisible(true);

	// ��ùؿ��趨�Ĺ���
    rapidjson::Value& monstersArray = document["monsters"];
    MonsterContainer::loadMonster(*this,  monstersArray);
	monsterCount = _monsterDatas->getCount();

	// ��ùؿ��趨������
	const rapidjson::Value& turretArray = document["turrets"];
	for (int i = 0; i < turretArray.Size(); i++)
	{
		// ���ÿһ��������name��gold��bulletName����
		std::string name = turretArray[i]["name"].GetString();
		int gold = turretArray[i]["gold"].GetInt();
		std::string bulletName = turretArray[i]["bulletName"].GetString();
		int type = turretArray[i]["type"].GetInt();
		// ����õ�name��gold��bulletName���ݴ洢���Զ���TurretData������
		auto turretData = TurretData::create();
		turretData->setName(name);
		turretData->setGold(gold);
		turretData->setType(type);
		turretData->setBulletName(bulletName);
		// ��TurretData�洢���ؿ��������ݵļ��ϵ���
		_turretDatas.pushBack(turretData);
	}
	// ��ù��ﾭ����·��
	const rapidjson::Value& pathArray = document["path"];
	for (int i = 0; i < pathArray.Size(); i++){
		// ���ÿ�����ﾭ��·����x��yֵ
		int x = pathArray[i]["x"].GetInt();
		int y = pathArray[i]["y"].GetInt();
		// ����x��yֵ����һ�����꣬��������һ����ͼ����
		Vec2 tilePoint = Vec2(x, y);
		// ����ͼ����ת����Ļ����
		Vec2 locationPoint = Coordinate::locationForTilePos(tilePoint,_tileMap);
		// ����Point���̳�Ref��Vector���ܴ洢�����������PointDelegate�����洢����
		auto pointDelegate = PointDelegate::create(locationPoint.x, locationPoint.y);
		// ��ÿһ����Ļ����洢��·�����ϵ���
		_pathPoints.pushBack(pointDelegate);
	}
	_castle = Princess::getInstance(_tileMap);
	/*************��ȡ�ؿ����ݽ���**********************/


	//����ͣ����ť
	auto pause_button = Button::create("pause.png");
	pause_button->setPosition(Vec2(40, visibleSize.height / 2 + 240));
	// ����ͣ����ť��Ӵ�������
	pause_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			// ��ͣ
			this->pause();
			for (unsigned int i = 0; i < _monsterVector.size(); i++) {
				auto monster = _monsterVector.at(i);
				monster->pause();
			}

		}
	});
	this->addChild(pause_button, 1);

	//���ָ�����ť
	auto start_button = Button::create("play.png");
	start_button->setPosition(Vec2(80, visibleSize.height / 2 + 240));
	start_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			//�ָ�
			this->resume();
			for (unsigned int i = 0; i < _monsterVector.size(); i++) {
				auto monster = _monsterVector.at(i);
				monster->resume();
			}
		}
	});
	this->addChild(start_button, 1);

	// ��ѡ��ؿ�����ť
	auto select_button = Button::create("btn-select.png");
	select_button->setPosition(Vec2(60, visibleSize.height / 2 + 175));
	// ��ѡ��ؿ�����ť��Ӵ�������
	select_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			// �ұ߻���糡Ч��
			//auto transition = TransitionSlideInR::create(2.0, LevelManager::createScene());//LevelManager::createScene());!!!
			// push��Ϸ����
			SimpleAudioEngine::getInstance()->stopBackgroundMusic(MUSIC_FILE);
			Director::getInstance()->pushScene(LevelManager::createScene());
			//Director::getInstance()->pushScene(transition);
		}
	});
	this->addChild(select_button, 1);

	/*************������Ļ�Ϸ���ʾ����Ϸ���ݿ�ʼ**********************/
	// ���Ͻ���ҽ������
	auto gold = Sprite::createWithSpriteFrameName("gold.png");
	gold->setPosition(50, _screenHeight*0.96);
	_tileMap->addChild(gold, 2);

	_goldLabel = Label::createWithSystemFont("200", "Arial", 32);
	_goldLabel->setColor(Color3B::RED);
	_goldLabel->setPosition(100, _screenHeight*0.96);
	_tileMap->addChild(_goldLabel, 2);

	// Ԥ��׼���ÿɽ�������� add.png
	addImage = Sprite::create("add.png");
	addImage->setScale(0.75);
	addImage->setVisible(false);
	_tileMap->addChild(addImage, 3);
	// ���ɽ����������ΪaddImage���ӽڵ�
	for (unsigned int i = 0; i<_turretDatas.size(); i++) {
		// ѭ���������ݵļ��ϻ�ȡÿһ������
		auto data = _turretDatas.at(i);
		// ��������
		auto turret = Turret::create(data->getName());
		turret->setGold(data->getGold());
		turret->setType(data->getType());
		turret->setScale(1.8);
		int rest = i % 2;
		int q = i / 2;
		turret->setPosition(60 * rest, (q + 1)*addImage->getContentSize().height*1.5 + 16);
		// ���������ΪaddImage���ӽڵ�
		addImage->addChild(turret);
	}

	// �����¼�������
	auto gameListener = EventListenerTouchOneByOne::create();
	// ��Ӧ�����¼�����
	gameListener->onTouchBegan = [=](Touch* touch, Event* event){
		// OpenGL����
		Vec2 touchLocation = touch->getLocation();
		// ���addImage�����ӽڵ㣨��������
		Vector<Node*> turrets = addImage->getChildren();
		for (unsigned int i = 0; i<turrets.size(); i++) {
			// ���ÿһ������
			auto node = turrets.at(i);
			Rect rect = Rect(0, 0, node->getContentSize().width, node->getContentSize().height);
			Turret* turret = (Turret*)node;
			// ���addImage��ʾ��˵������ǵڶ��δ�����ѡ��������
			if (addImage->isVisible())
			{
				// ���������ҽ�������������false
				if (turret->getGold() > _goldValue) {
					_isSelect = false;
					addImage->setVisible(false);
					return false;
				}
				else{
					// ������Խ�������������_isSelect=true
					if (rect.containsPoint(node->convertToNodeSpace(touchLocation))) {
						addImage->setTag(i);
						_isSelect = true;
						return true;
					}
				}
			}
			// ����ǵ�һ�δ���
			else
			{
				// �������������Ҳ�������������͸���ȣ���ʾ���ɽ���
				if (turret->getGold() > _goldValue) {
					turret->setOpacity(100);
				}
			}
		}
		return true;
	};
	gameListener->onTouchEnded = [=](Touch *touch, Event *event){
		// OpenGL����
		Vec2 touchLocation = touch->getLocation();
		// ���Node������
		Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
		auto tempLocation = Coordinate::tileCoordForPosition(nodeLocation,_tileMap);
		tempLocation.x -= static_cast<int>(tempLocation.x) % 2 - 1;
		tempLocation.y -= static_cast<int>(tempLocation.y) % 2 - 1;
		auto addLocation = Coordinate::locationForTilePos(tempLocation, _tileMap);
		// ͬһ��λ���ϲ��ܷ�������
		for (int i = 0; i < _turretVector.size(); i++) {
			auto temp = _turretVector.at(i);
			if (temp->getBoundingBox().containsPoint(addLocation)) {
				return;
			}
		}
		// _isSelectΪtrue���û���ѡ����������false���û�ѡ����������λ��
		if (_isSelect) {
			_isSelect = false;
			// ����һ����������
			TurretData *data = _turretDatas.at(addImage->getTag());
			Turret *turret = Turret::create(data->getName());
			turret->setScale(1.4);
			// ������������addImage��λ����
			turret->setPosition(addImage->getPosition());
			turret->setBulletName(data->getBulletName());
			turret->setType(data->getType());
			turret->setGold(data->getGold());
			_tileMap->addChild(turret, 1);
			// ����addImage
			addImage->setVisible(false);
			// ��������ӵ���������
			_turretVector.pushBack(turret);
			// ���ѽ��
			_goldValue -= data->getGold();
		}
		else{
			// ���׼������������λ�ò����ϰ���ʱ
			if (!Coordinate::getCollidable(nodeLocation,_tileMap, _collidable)) {
				// addImage��ʾ
				addImage->setVisible(true);
				// ����addImageλ��Ϊ����λ��
				//				nodeLocation.x -= static_cast<int>(nodeLocation.x) % 60 - 20;
				//				nodeLocation.y -= static_cast<int>(nodeLocation.y) % 60 - 40;
				addImage->setPosition(addLocation);
			}
		}
	};


	// ��ӳ��������¼�������
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener, this);

	// ��ʼ��Ϸ������3��
	Label* label1 = Label::createWithSystemFont("1", "Arial-BoldMT", 150);
	Label* label2 = Label::createWithSystemFont("2", "Arial-BoldMT", 150);
	Label* label3 = Label::createWithSystemFont("3", "Arial-BoldMT", 150);
	label1->setColor(Color3B::BLUE);
	label2->setColor(Color3B::BLUE);
	label3->setColor(Color3B::BLUE);
	label1->setPosition(_screenWidth / 2, _screenHeight / 2);
	label2->setPosition(_screenWidth / 2, _screenHeight / 2);
	label3->setPosition(_screenWidth / 2, _screenHeight / 2);
	// ����Label����ʾ
	label1->setVisible(false);
	label2->setVisible(false);
	label3->setVisible(false);
	_tileMap->addChild(label1, 2);
	_tileMap->addChild(label2, 2);
	_tileMap->addChild(label3, 2);
	// ��ɵ�����Sequence������3��2��1��ÿ��1����ʾ1�����֣�ͬʱɾ��֮ǰ��ʾ������
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
	// ִ�е�������
	this->runAction(countdown);

	// ���ű�������
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_FILE, true);
	// ���ñ������ִ�С
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);

	/*************������Ļ�Ϸ���ʾ����Ϸ���ݽ���**********************/

	return true;
}

// ��Ϸ��ѭ��
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


// ��ö�����������
Animate* Game::getAnimateByName(std::string animName, float delay, int animNum)
{
	// ��������
	Animation* animation = Animation::create();
	// ѭ���Ӿ���֡�����л�ȡ��ͼƬ�������Ӧ�ľ���֡��ɶ���
	for (unsigned int i = 1; i <= animNum; i++){
		// ��ȡ����ͼƬ���ƣ�����pest1.png
		std::string frameName = animName;
		frameName.append(StringUtils::format("%d.png", i));
		// ������ͼƬ���Ϊ����֡��������֡��
		animation->addSpriteFrameWithFile(frameName.c_str());
	}
	// ���ö������ŵ�����
	animation->setDelayPerUnit(delay);
	// �þ�������ڶ���ִ�����ָ������״̬
	animation->setRestoreOriginalFrame(true);
	// ���ض�����������
	Animate* animate = Animate::create(animation);
	return animate;
}

void Game::updateHUD(float delta)
{
	// ���½������
	_goldLabel->setString(StringUtils::format("%d", _goldValue));
	// ����ÿһ������Ѫ��λ�ã�Ѫ����������ƶ�
	for (unsigned int i = 0; i < _monsterVector.size(); i++) {
		auto monster = _monsterVector.at(i);
		// ������ﲻΪnullptr�����ҹ�������ֵ��Ϊ0��ʱ��
		if (monster != nullptr && monster->getLifeValue() > 0) {
			// ���ﾫ������
			Vec2 pos = monster->getPosition();
			// ���ù���Ѫ����ʾ������λ��
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
	// ���¹��ﲨ����
	_currNumLabel->setString(StringUtils::format("%d", _currNum));
}


// �������·�����ƶ�
void Game::monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector, MonsterBase* monster){
	// ���嶯�����ϣ�����������moveTo����
	Vector<FiniteTimeAction*> actionVector;
	// ѭ��·���㼯�ϣ��������MoveTo��������ҽ�ִ�ж��MoveTo��������ƶ�
	for (int i = 0; i < pathVector.size(); i++) {
		// �����Ҫ�ƶ���ÿһ����
		PointDelegate* pd = pathVector.at(i);
		Vec2 glPoint = Vec2(pd->getX(), pd->getY());
		// ����MoveToһ���������þ�������ƶ���ָ����λ��
		MoveTo* moveTo = MoveTo::create(0.2f, glPoint);
		// ��MoveTo������ӵ���ʱ����
		actionVector.pushBack(moveTo);
	}
	// ��ȡ��������
	std::string monsterName = monster->getName();
	size_t iPos = monsterName.find(".png");
	monsterName = monsterName.substr(0, iPos);
	// ����getAnimateByName���������Ҿ����ƶ�����
	auto animate = getAnimateByName(monsterName, 0.5f, 5);
	// ����һ���������ظ�ִ��Animate����
	auto repeatanimate = RepeatForever::create(animate);
	// ��Ҿ����ظ�ִ�ж�������
	monster->runAction(repeatanimate);
	// �����ص���������MoveTo������ɺ���ָ����վ��״̬
	auto callfunc = CallFunc::create([=]{
		// ֹͣ����
		monster->stopAction(repeatanimate);
	});
	actionVector.pushBack(callfunc);
	// ��˳��ִ�ж��������еĶ���
	auto sequence = Sequence::create(actionVector);
	// ִ��һϵ�еĶ���
	monster->runAction(sequence);
}

// �������
void Game::detectionTurret(float delta)
{
	// ������������
	for (unsigned int i = 0; i < _turretVector.size(); i++) {
		// ���ÿһ������
		auto turret = _turretVector.at(i);
		turret->setTime(turret->getTime() + 1);
		// ��Ϸ�趨����һ��ֻ�ܷ���һö�ڵ����������û�й����ڵ������ڵ��Ѿ��������
		if (turret->getTime() > 40)
		{
			// �������Ｏ��
			turret->setTime(0);
			for (unsigned int j = 0; j < _monsterVector.size(); j++) {
				auto monster = _monsterVector.at(j);
				// �������Ƿ��������Ĺ�����Χ
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
// ����ڵ��͹������ײ
void Game::collisionDetection(float delta)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// ������������
	for (unsigned int i = 0; i < _monsterVector.size(); i++)
	{
		MonsterBase* monster = _monsterVector.at(i);
		// �����ڵ�����
		for (unsigned int j = 0; j < _bulletVector.size(); j++) {
			auto bullet = _bulletVector.at(j);
			// ����������ڵ���������ײ
			if (monster->getBoundingBox().intersectsRect(bullet->getBoundingBox())){
				// ���ù��������ֵ1

				monster->getInjured();
				// ���¹���Ѫ��
				
				
				// ���ڵ�������ɾ���ڵ�
					_bulletVector.eraseObject(bullet);
					// �޸��ڵ�������
					bullet->setShoot(false);
					// �ӵ�ͼ��ɾ���ڵ�
					_tileMap->removeChild(bullet);
					break;
			}
			else
			{
				// ����ڵ����﹥�����꣬�������Ѿ��߿������ڵ�û�д��й���
				if (bullet->getNumberOfRunningActions() == 0){
					// ɾ���ڵ�
					_bulletVector.eraseObject(bullet);
					// �޸��ڵ�������
					bullet->setShoot(false);
					// �ӵ�ͼ��ɾ���ڵ�
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
		// �����������ֵΪ0
		if (monster->getLifeValue() <= 0) {
			// ���Ź��ﱻ�������Ч
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE1);
			// �ӹ��Ｏ����ɾ������
			_monsterVector.eraseObject(monster);
			// �������
			_goldValue += monster->getGold();
			// �ӵ�ͼ��ɾ������
			_tileMap->removeChild(monster);
			_aliveCount--;
			if (_aliveCount == 0)
			{
				_isThisFinish = true;
			}
		}
	}
}

// ��⹫��
void Game::detectionPrincess(float delta)
{
	// ����ÿһ������
	for (unsigned int j = 0; j < _monsterVector.size(); j++) {
		auto monster = _monsterVector.at(j);
		// ��������빥����������Ϸʧ��
		if (monster->getBoundingBox().containsPoint(_castle->getPosition())){
			// ��Ϸʧ��
			gameOver(0);
		}
	}
}

// ��Ϸ����
void Game::gameOver(int tag)
{

	// ֹͣ��Ϸ��ѭ��
	this->unscheduleUpdate();
	// ֹͣ���й��ﶯ��
	for (unsigned int i = 0; i < _monsterVector.size(); i++) {
		auto monster = _monsterVector.at(i);
		monster->stopAllActions();
	}
	// ɾ����Ļ�����ڵ�
	for (unsigned int i = 0; i < _bulletVector.size(); i++) {
		auto bullet = _bulletVector.at(i);
		bullet->stopAllActions();
		_tileMap->removeChild(bullet);
	}
	_bulletVector.clear();

	// ѡ����Ļ��
	auto temp = Sprite::createWithSpriteFrameName("block.png");
	temp->setPosition(_screenWidth / 2, _screenHeight / 2);
	temp->setColor(Color3B::GRAY);
	_tileMap->addChild(temp, 2);

	// ֹͣ��������
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(MUSIC_FILE);

	// �㹲������10������!
//	std::string messageString = "�㹲������";
//	sscanf(messageString, "");
	auto messageLabel = Label::createWithSystemFont(StringUtils::format("You have repelled [%d]\n waves of monsters!", _currNum), "Arial", 38);
	messageLabel->setColor(Color3B::WHITE);
	messageLabel->setPosition(_screenWidth / 2, _screenHeight / 2 + 50);
	_tileMap->addChild(messageLabel, 4);

	// ��ѡ��ؿ�����ť
	auto select_button = Button::create("btn-select.png");
	select_button->setPosition(Vec2(_screenWidth / 2 - 100, _screenHeight / 2 - 80));
	select_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			// �ұ߻���糡Ч��
			auto transition = TransitionSlideInR::create(2.0, LevelManager::createScene());
			// push��Ϸ����
			Director::getInstance()->pushScene(transition);
		}
	});
	this->addChild(select_button, 3);

	// ������һ�Ρ���"������һ��"��ť
	auto play_button = Button::create();
	if (tag == 1){
		// ������һ��
		play_button->loadTextures("btn-next.png", "btn-next.png");
		_currentLevel++;

		

	}
	else{
		// ����һ��
		// ������Ϸʧ����Ч
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
				// TransitionSplitRows ���һ����糡Ч��
				auto transition = TransitionSplitRows::create(2.0, Game::createSceneWithLevel(_currentLevel));
				// push��Ϸ����
				Director::getInstance()->pushScene(transition);
			}
		}
	});
	play_button->setPosition(Vec2(_screenWidth / 2 + 100, _screenHeight / 2 - 80));
	this->addChild(play_button, 3);
}