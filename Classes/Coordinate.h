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
	// TileMap����ת��ΪOpenGL����
	static Vec2 locationForTilePos(Vec2 pos, TMXTiledMap* _tileMap);
	// ��OpenGL����ת��ΪTileMap����
	static Vec2 tileCoordForPosition(Vec2 position, TMXTiledMap* _tileMap);
	// ���������Ƿ��ϰ�
	static bool getCollidable(Vec2 position, TMXTiledMap* _tileMap, TMXLayer* _collidable);
};
#endif
