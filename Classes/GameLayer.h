#ifndef __GAMELAYER_SCENE_H__
#define __GAMELAYER_SCENE_H__

#include "cocos2d.h"
#include "Number.h"
enum Dir {
	left,
	right,
	up, 
	down
};
class GameLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
	void initNumber();
	void update(float dt);
	void removeZero(bool inverted, bool again);
	void getDataForColORRow(int arr[][4], int getNum);
	void merge(bool inverted);
	void reduction(int arr[][4], int reductionNum);
	void setKeyboardEnable(bool enable);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event*);
	void arrClearr();
	void randomCreateNum();
	GameLayer();
	~GameLayer();
	CREATE_FUNC(GameLayer);
private:
	Number *number[4][4];
	Dir dir;
	int map[4][4] = {
	{ 2, 2, 4, 8 },
	{ 2, 4, 4, 4 },
	{ 0, 8, 4, 0 },
	{ 2, 4, 0, 4 }
	};
	int temp[4]{ 0 };
	bool isInput = false;
};

#endif // __GAMELAYER_SCENE_H__
