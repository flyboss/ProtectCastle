#include"Coordinate.h"


// TileMap坐标转换为OpenGL坐标
Vec2 Coordinate::locationForTilePos(Vec2 pos, TMXTiledMap* _tileMap)
{
	int x = (int)(pos.x*(_tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	double pointHeight = _tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int)((_tileMap->getMapSize().height * pointHeight) - (pos.y * pointHeight));
	return Vec2(x, y);
}

// 将OpenGL坐标转换为TileMap坐标
Vec2 Coordinate::tileCoordForPosition(Vec2 position, TMXTiledMap* _tileMap) {
	// 玩家位置的x除以地图的宽，得到的是地图横向的第几个格子（tile）
	int x = (int)(position.x / (_tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	// 玩家位置的y除以地图的高，得到的是地图纵向第几个格子（tile），
	double pointHeight = _tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int)((_tileMap->getMapSize().height * pointHeight - position.y) / pointHeight);
	return Vec2(x, y);
}


// 检测坐标点是否障碍
bool Coordinate::getCollidable(Vec2 position, TMXTiledMap* _tileMap, TMXLayer* _collidable)
{
	// 将屏幕坐标转成地图坐标，用于判断是否可以放置炮塔
	Vec2 tilePos = Coordinate::tileCoordForPosition(position, _tileMap);
	// 如果触摸点是不可放置炮塔（即有障碍物）的位置，则直接return
	int tileGid = _collidable->getTileGIDAt(tilePos);
	if (tileGid) {
		// 使用GID来查找指定tile的属性，返回一个Value
		Value properties = _tileMap->getPropertiesForGID(tileGid);
		// 返回的Value实际是一个ValueMap
		ValueMap map = properties.asValueMap();
		// 查找ValueMap，判断是否有障碍物，如果有，直接返回
		std::string value = map.at("collidable").asString();
		if (value.compare("true") == 0) {
			return true;
		}
	}
	return false;
}
