#ifndef __ProtectCastle__Game__
#define __ProtectCastle__Game__

#include <iostream>
#include "cocos2d.h" 
#include "PointDelegate.h"
#include "SpriteBase.h"
#include "Monster.h"
#include "Turret.h"
#include "Bullet.h"
#include "SimpleAudioEngine.h"
#include "MonsterDataOfWave.h"
#include "TurretData.h"
#include "json/document.h"
#include "LevelManager.h"
#include "GameMenu.h"
#include "GameEnd.h"
#include "Coordinate.h"
#include "Princess.h"
#include "MonsterFactory.h"

#include "MonsterContainer.h"

USING_NS_CC;
using namespace CocosDenshion;

// 游戏逻辑类
class Game : public cocos2d::Layer
{
public:
	TMXTiledMap* _tileMap;  // 地图
	TMXLayer* _collidable;  // 障碍层
	std::string _tileFile; // 关卡地图名称
	int _number; // 关卡共出现多少波怪物
	int _currentCount; // 当前出现怪物数量
	int _aliveCount;
	bool _isThisFinish;
	int _currNum; // 当前出现怪物波数
	int _goldValue; // 玩家当前的金币值
	Sprite* _castle; // 公主
	int _screenWidth, _screenHeight;  // 屏幕宽度和高度
	int _count;  // 游戏帧计数器
	int _delivery; // 出现怪物取模基数

	Sprite* addImage;
	MonsterDataOfWave* _monsterDatas;
//	Vector<MonsterData*> _monsterDatas; // 当前关卡的怪物信息
	Vector<MonsterDataOfWave*> _monsterAllDatas;
//	std::vector<Vector<MonsterData*>> _monsterAllDatas;
//	std::vector<int> _monsterCount;
	Vector<TurretData*> _turretDatas; // 当前关卡的炮塔信息
	Vector<PointDelegate*> _pathPoints; // 记录有效路径的点

	Label* _numberLabel; // 显示怪物波数的Label
	Label* _currNumLabel; // 显示当前怪物波数的Label
	Label* _goldLabel; // 显示当前玩家金币的Label

//	std::vector<Vector<Monster*>> _monsterVectorS;
	Vector<MonsterBase*> _monsterVector;  // 存储出现怪物的集合
	bool _isFinish = false; // 关卡所有怪物是否全部出现标记
	bool _isSelect = false; // 判断用户触碰是选择位置还是建造炮塔

	Vector<Turret*> _turretVector; // 炮塔集合，存放已经添加的炮塔
	Vector<Bullet*> _bulletVector; // 炮弹集合


	/**
	* 静态函数，创建Scene场景
	* 注意：在Cocos2d-x中没有Cocos2d-iPhone中的“id”,建议返回类实例的指针返回类实例的指针
	* selectLevel：当前关卡数
	*/
	static cocos2d::Scene* createSceneWithLevel(int selectLevel);

	virtual bool init();

	bool initialize(int currentLevel);
	//文件改到了Coordinate
	// TileMap坐标转换为OpenGL坐标
	//Point locationForTilePos(Point pos);
	// 将OpenGL坐标转换为TileMap坐标
	//Point tileCoordForPosition(Point position);
	// 回调更新函数，该函数每一帧都会调用
	virtual void update(float delta);
	/**
	* 获得动画动作
	* animName：动画帧的名字
	* delay：动画帧与帧之间的间隔时间
	* animNum：动画帧的数量
	*/
	Animate* getAnimateByName(std::string animName, float delay, int animNum);
	/**
	* 怪物根据路径点移动
	* pathVector：从关卡json文件中获取的路径集合
	* monster：移动的怪物
	*/
	virtual void monsterMoveWithWayPoints(Vector<PointDelegate*> pathVector, MonsterBase* monster);
	// 更新HUD
	virtual void updateHUD(float delta);
	// 更新计数器
	virtual void updateLable(float delta);

	// 检测炮塔
	virtual void detectionTurret(float delta);
	// 检测碰撞
	virtual void collisionDetection(float delta);
	// 检测公主
	virtual void detectionPrincess(float delta);
	// 游戏结束，tag，0表示游戏失败，1表示游戏胜利
	virtual void gameOver(int tag);
	// 使用CREATE_FUNC宏创建当前类的对象
	CREATE_FUNC(Game);

};

#endif