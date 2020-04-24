#include "Number.h"

Number* Number::create(int number) {
	Number* ret = new (std::nothrow)Number();
	if (ret && ret->init(number)) {
		ret->autorelease();
	} else {
		delete ret;
		ret = nullptr;
	}
	return ret;
}

bool Number::init(int number) {
	char filename[40];
	sprintf_s(filename, "image/%d.png", number);
	if (!Sprite::initWithFile(filename)) {
		return false;
	}
	return true;
}

void Number::setImage(int number) {
	char filename[40];
	sprintf_s(filename, "image/%d.png", number);
	this->setTexture(filename);
}