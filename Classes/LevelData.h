#ifndef __ProtectCastle__LevelData__
#define __ProtectCastle__LevelData__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

// LevelData�࣬�����洢�ؿ�������Ϣ
// {"number": 1,"bg":"level1.png","data":"1.data"}
class LevelData : public Ref
{
protected:
	unsigned int _number; // �ؿ�����
	std::string _bg; // ����ͼƬ
	std::string _data;  // �ؿ��������ļ�
public:
	// ��̬��create����
	static LevelData* create(unsigned int number, std::string bg, std::string data);

	// setter��getter����
	void setNumber(unsigned int number);

	unsigned int getNumber();

	void setBg(std::string bg);

	std::string getBg();

	void setData(std::string data);

	std::string getData();
};


#endif