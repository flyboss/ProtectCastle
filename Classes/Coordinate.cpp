#include"Coordinate.h"


// TileMap����ת��ΪOpenGL����
Vec2 Coordinate::locationForTilePos(Vec2 pos, TMXTiledMap* _tileMap)
{
	int x = (int)(pos.x*(_tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	double pointHeight = _tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int)((_tileMap->getMapSize().height * pointHeight) - (pos.y * pointHeight));
	return Vec2(x, y);
}

// ��OpenGL����ת��ΪTileMap����
Vec2 Coordinate::tileCoordForPosition(Vec2 position, TMXTiledMap* _tileMap) {
	// ���λ�õ�x���Ե�ͼ�Ŀ��õ����ǵ�ͼ����ĵڼ������ӣ�tile��
	int x = (int)(position.x / (_tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	// ���λ�õ�y���Ե�ͼ�ĸߣ��õ����ǵ�ͼ����ڼ������ӣ�tile����
	double pointHeight = _tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int)((_tileMap->getMapSize().height * pointHeight - position.y) / pointHeight);
	return Vec2(x, y);
}


// ���������Ƿ��ϰ�
bool Coordinate::getCollidable(Vec2 position, TMXTiledMap* _tileMap, TMXLayer* _collidable)
{
	// ����Ļ����ת�ɵ�ͼ���꣬�����ж��Ƿ���Է�������
	Vec2 tilePos = Coordinate::tileCoordForPosition(position, _tileMap);
	// ����������ǲ��ɷ��������������ϰ����λ�ã���ֱ��return
	int tileGid = _collidable->getTileGIDAt(tilePos);
	if (tileGid) {
		// ʹ��GID������ָ��tile�����ԣ�����һ��Value
		Value properties = _tileMap->getPropertiesForGID(tileGid);
		// ���ص�Valueʵ����һ��ValueMap
		ValueMap map = properties.asValueMap();
		// ����ValueMap���ж��Ƿ����ϰ������У�ֱ�ӷ���
		std::string value = map.at("collidable").asString();
		if (value.compare("true") == 0) {
			return true;
		}
	}
	return false;
}
