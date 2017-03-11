#include"Princess.h"

Princess::Princess()
{
}
Princess* Princess::_princess = nullptr;
Princess* Princess::getInstance(TMXTiledMap* _tileMap)
{
	// 获得名称为princessObject的对象层
	auto princessObject = _tileMap->getObjectGroup("princessObject");
	// 获得名称为princess的地图对象，该对象在地图的位置就是公主出现的位置
	ValueMap princessValueMap = princessObject->getObject("princess");
	int princessX = princessValueMap.at("x").asInt();
	int princessY = princessValueMap.at("y").asInt();
	_princess = new Princess();
	if (_princess && _princess->initWithFile("princess.png"))
	{
		_princess->autorelease();
	}
	// 将地图中对象的x和y值作为公主出现的位置
	_princess->setPosition(princessX, princessY);
	// 将公主添加为地图的子节点
	_tileMap->addChild(_princess, 2);
	return _princess;
}