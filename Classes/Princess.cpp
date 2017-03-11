#include"Princess.h"

Princess::Princess()
{
}
Princess* Princess::_princess = nullptr;
Princess* Princess::getInstance(TMXTiledMap* _tileMap)
{
	// �������ΪprincessObject�Ķ����
	auto princessObject = _tileMap->getObjectGroup("princessObject");
	// �������Ϊprincess�ĵ�ͼ���󣬸ö����ڵ�ͼ��λ�þ��ǹ������ֵ�λ��
	ValueMap princessValueMap = princessObject->getObject("princess");
	int princessX = princessValueMap.at("x").asInt();
	int princessY = princessValueMap.at("y").asInt();
	_princess = new Princess();
	if (_princess && _princess->initWithFile("princess.png"))
	{
		_princess->autorelease();
	}
	// ����ͼ�ж����x��yֵ��Ϊ�������ֵ�λ��
	_princess->setPosition(princessX, princessY);
	// ���������Ϊ��ͼ���ӽڵ�
	_tileMap->addChild(_princess, 2);
	return _princess;
}