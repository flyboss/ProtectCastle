#include "GameBoss.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

static int monsterCount = 1; // ÿһ�����ֶ��ٹ���
static int _currentLevel = 1;  // ��ǰ�ؿ�

Scene* GameBoss::createSceneWithLevel(int selectLevel)
{
	// ��ùؿ���
	_currentLevel = selectLevel;
	// ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto scene = Scene::create();
	// ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto layer = GameBoss::create();
	// ��Game����Ϊ�ӽڵ���ӵ�����
	scene->addChild(layer);
	// ���س�������
	return scene;
}

bool GameBoss::init()
{
	if (!initialize(_currentLevel))
	{
		return false;
	}
	_castle->setVisible(false);
	// ��ʱ��
	_time = 60;
	_currNumLabel = Label::createWithSystemFont(StringUtils::format("%d seconds left~", _time), "Arial", 32);
	_currNumLabel->setColor(Color3B::BLUE);
	_currNumLabel->setPosition(_screenWidth*0.5, _screenHeight*0.96);
	_tileMap->addChild(_currNumLabel, 2);
	//��ʼ��Ϸ��ѭ��
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
	// _isFinish���Ϊfalse��ʾ���Գ��ֹ���
	if (!_isFinish) {
		// _delivery����Ϊȡģ������ͨ�����øñ���ֵ�����Կ��ƹ������Ƶ��
		if (_count % _delivery == 0) {
			// ��ǰ���ֹ���������1
			_currentCount++;
			// �����ǰ���ֹ�����������ÿһ�����ֶ��ٹ����ʾһ������
			if (_currentCount > monsterCount) {
				// �ؿ������ܲ�����1
				_number -= 1;
				// ���ؿ������ܲ���Ϊ0
				if (_number == 0) {
					// ���ñ�ǣ���ʾ�ؿ����й���ȫ���������
					_isFinish = true;
				}
				else{
					// ���¿�ʼһ������
					_currentCount = 0; // ��ǰ���ֹ�����������
					_currNum++; // ��ǰ���ֹ��ﲨ����1
					_monsterDatas = _monsterAllDatas.at(_currNum - 1);
					monsterCount = _monsterDatas->getCount();
					_delivery = _monsterDatas->getDelivery();
				}
			}
			else{
				// ����TMXTiledMap��getObjectGroup������ȡ�����
				auto pestObject = _tileMap->getObjectGroup("monsterObject");
				// ���ݶ������ƻ�ȡ�������Ϣ
				ValueMap pestValueMap = pestObject->getObject("monster");
				// ��ȡ��ͼ�����õ�player�����x��yֵ
				int pestX = pestValueMap.at("x").asInt();
				int pestY = pestValueMap.at("y").asInt();
				// һ���ؿ����ܳ��ֶ��ֹ��������json�ļ�������
				int random = rand() % _monsterDatas->size();
				auto monsterData = _monsterDatas->at(random);
				// ���ݹ������ƴ�������
				auto monster = Monster::create(monsterData->getName());
				_aliveCount++;

				// ���ù������ơ�����ֵ���������Ľ��ֵ�͹����������
				monster->setName(monsterData->getName());
				monster->setLifeValue(monsterData->getLifeValue());
				monster->setGold(monsterData->getGold());
				monster->setScale(0.7);
				monster->setPosition(pestX, pestY);

				// ����������
				auto loadingBar = LoadingBar::create("planeHP.png");
				// ���ý���������
				loadingBar->setDirection(LoadingBar::Direction::LEFT);
				// ���ý�������С
				loadingBar->setScale(0.06f, 0.1f);
				// ���ý������ٷֱ�
				loadingBar->setPercent(100);
				// ������������Ϊ����Ѫ��
				monster->setHP(loadingBar);
				// ��ӹ���Ѫ��
				_tileMap->addChild(monster->getHP(), 2);
				// ����Ѫ��������
				monster->setHPInterval(100 / monster->getLifeValue());
				// ��������Ϊ��ͼ�ӽڵ�
				_tileMap->addChild(monster, 2);
				// ��������ӵ��ѳ��ֹ�������
				_monsterVector.pushBack(monster);
				// �������·�����ƶ�
				monsterMoveWithWayPoints(_pathPoints, monster);
			}
		}
	}
	else
	{
		// _isFinish���Ϊtrue��ʾ�ؿ����й����Ѿ��������
		if (_monsterVector.size() <= 0) {
			// ��Ϸʤ��
			gameOver(1);
		}
	}
}

void GameBoss::monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector, Monster* monster){
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
	auto repeatesequence = RepeatForever::create(sequence);
	// ִ��һϵ�еĶ���
	monster->runAction(repeatesequence);
}

void GameBoss::updateLable(float delta)
{
	// ���¼�ʱ��
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
				// TransitionSplitRows ���һ����糡Ч��
				auto transition = TransitionSplitRows::create(2.0, GameBoss::createSceneWithLevel(_currentLevel));
				// push��Ϸ����
				Director::getInstance()->pushScene(transition);
			}
		}
	});
	play_button->setPosition(Vec2(_screenWidth / 2 + 100, _screenHeight / 2 - 80));
	this->addChild(play_button, 3);
}
