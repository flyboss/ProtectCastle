#ifndef __ProtectCastle__Coordinate__
#define __ProtectCastle__Coordinate__

#include "ui/CocosGUI.h"
#include "cocos2d.h"
USING_NS_CC;
class Coordinate
{
private:
public:
	Coordinate() {}
	~Coordinate() {}
	// TileMap坐标转换为OpenGL坐标
	static Vec2 locationForTilePos(Vec2 pos, TMXTiledMap* _tileMap);
	// 将OpenGL坐标转换为TileMap坐标
	static Vec2 tileCoordForPosition(Vec2 position, TMXTiledMap* _tileMap);
	// 检测坐标点是否障碍
	static bool getCollidable(Vec2 position, TMXTiledMap* _tileMap, TMXLayer* _collidable);
};
#endif
