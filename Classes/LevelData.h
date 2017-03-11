#ifndef __ProtectCastle__LevelData__
#define __ProtectCastle__LevelData__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

// LevelData类，用来存储关卡数据信息
// {"number": 1,"bg":"level1.png","data":"1.data"}
class LevelData : public Ref
{
protected:
	unsigned int _number; // 关卡数量
	std::string _bg; // 背景图片
	std::string _data;  // 关卡的数据文件
public:
	// 静态的create函数
	static LevelData* create(unsigned int number, std::string bg, std::string data);

	// setter和getter函数
	void setNumber(unsigned int number);

	unsigned int getNumber();

	void setBg(std::string bg);

	std::string getBg();

	void setData(std::string data);

	std::string getData();
};


#endif