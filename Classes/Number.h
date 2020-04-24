#pragma once
#include "cocos2d.h"
class Number : public cocos2d::Sprite{
public :
	static Number* create(int number);
	bool init(int number);
	void setImage(int number);
};

