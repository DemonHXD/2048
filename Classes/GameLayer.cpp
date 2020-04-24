#include "GameLayer.h"
#include <cstdlib>
#include <ctime>

using namespace cocos2d;
USING_NS_CC;

static const int ArrSize = 4;

//***************************2048核心算法**********************************//
		/*
		 * 上移
		 * --从上到下获取列数据，形成一维数组
		 * 2 2 0 0 -> 4 0 0 0
		 * 2 2 2 0 -> 4 0 2 0 -> 4 2 0 0
		 * 2 0 2 0 -> 2 2 0 0 -> 4 0 0 0
		 * --合并数据
		 *  --去零：将0元素移至末尾
		 *  --相邻 相同则合并(将后一个元素累加到前一个元素上，后一个元素清0)
		 *  --去零：将0元素移至末尾
		 * --将一维数组元素还原至原列
		 *
		 *
		 * 下移1.0
		 * --从上到下获取列数据，形成一维数组
		 * --合并数据
		 *  --去零：将0元素移至开头
		 *  --相邻 相同则合并(将前一个元素累加到后一个元素上，前一个元素清0)
		 *  --去零：将0元素移至开头
		 * --将一维数组元素还原至原列
		 *
		 * 下移2.0
		 * --从下到上获取列数据，形成一维数组
		 * --合并数据
		 *  --去零：将0元素移至末尾
		 *  --相邻 相同则合并(将后一个元素累加到前一个元素上，后一个元素清0)
		 *  --去零：将0元素移至末尾
		 * --将一维数组元素还原至原列
		 *
		 * 左移
		 *
		 * 右移
		 *
		 *
		 * 1.定义去零方法(针对一维数组)：将0元素移至末尾
		 * 2.合并数据方法(针对一维数组)
		 *  --去零：将0元素移至末尾
		 *  --相邻 相同则合并(将后一个元素累加到前一个元素上，后一个元素清0)
		 *  --去零：将0元素移至末尾
		 * 3.上移
		 *  --从上到下获取列数据，形成一维数组
		 *  --调用合并数据方法
		 *  --将一维数组元素还原至原列
		 * 4.上移
		 *  --从下到上获取列数据，形成一维数组
		 *  --调用合并数据方法
		 *  --将一维数组元素还原至原列
		 * 5.左移
		 * 6.右移
		 */


GameLayer::GameLayer():dir(up) {

}
GameLayer::~GameLayer() {

}

Scene* GameLayer::createScene() {
	auto scene = Scene::create();
	auto layer = GameLayer::create();
	scene->addChild(layer);
	return scene;
}

bool GameLayer::init() {
	if (!Layer::init()) {
		return false;
	}

	//产生随机数种子
	srand((int)time(0));

	Size size = Director::getInstance()->getVisibleSize();

	//创建背景
	Sprite* bg = Sprite::create("image/map.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg, -1);

	//初始化数字
	initNumber();
	
	//开启默认调度器
	scheduleUpdate();

	//开启键盘监听
	setKeyboardEnable(true);

	return true;
}

/*
	默认调度器
*/
void GameLayer::update(float dt) {
	if (isInput) {
		//正向获取数据还是反向
		bool inverted;
		switch (dir) {
		case left:
		case up:
			inverted = true;
			break;
		case down:		
		case right:
			inverted = false;
			break;
		}
		for (int i = 0; i < ArrSize; i++) {
			//获取数据
			getDataForColORRow(map, i);
			//合并
			merge(inverted);
			//还原
			reduction(map, i);
			//清空
			arrClearr();
		}
		//创建随机数字
		randomCreateNum();
		for (size_t i = 0; i < ArrSize; i++) {
			for (size_t j = 0; j < ArrSize; j++) {
				number[i][j]->setImage(map[i][j]);
				number[i][j]->setPosition(Vec2(113 * j + 7, 113 * (ArrSize - i - 1) + 7));
			}
		}
		isInput = false;
	}
	
}

/*
	初始化棋盘数字
*/
void GameLayer::initNumber() {
	//开局随机先创建两个数字
	randomCreateNum();
	randomCreateNum();
	for (size_t i = 0; i < ArrSize; i++) {
		for (size_t j = 0; j < ArrSize; j++) {
			number[i][j] = Number::create(map[i][j]);
			number[i][j]->setAnchorPoint(Vec2(0, 0));
			number[i][j]->setPosition(Vec2(113 * j + 7, 113 * (ArrSize - i - 1) + 7));
			this->addChild(number[i][j], 1);
		}
	}
}


/*
	获取原行或原列数据
*/
void GameLayer::getDataForColORRow(int arr[][ArrSize], int getNum) {
	for (int i = 0; i < ArrSize; i++) {
		switch (dir) {
		case left:
			temp[i] = arr[getNum][i];
			break;
		case right:
			temp[i] = arr[getNum][ArrSize - i - 1];
			break;
		case down:
			temp[i] = arr[ArrSize - i - 1][getNum];
			break;
		case up:
			temp[i] = arr[i][getNum];
			break;
		}
	}
}

/*
	合并方法
*/
void GameLayer::merge(bool inverted) {
	//第一次去零
	removeZero(inverted, false);
	for (int i = 0; i < ArrSize - 1; i++) {
		if (temp[i] == temp[i + 1]) {
			temp[i] += temp[i + 1];
			temp[i + 1] = 0;
		}
	}
	//第二次去零
	removeZero(inverted, true);
}

/*
	去0方法
*/
void GameLayer::removeZero(bool inverted, bool again) {
	int arr[ArrSize]{ 0 };
	int j = 0;
	for (int i = 0; i < ArrSize; i++) {
		if (temp[i] != 0) {
			arr[j++] = temp[i];
			temp[i] = 0;
		}
	}
	for (int i = 0; i < j; i++) {
		if (inverted) {
			temp[i] = arr[i];
		} else {
			if (!again) {
				temp[i] = arr[i];
			} else {
				temp[ArrSize - i - 1] = arr[i];
			}
		}
	}
}

/*
	将一维数组还原至原列或原行
*/
void GameLayer::reduction(int arr[][ArrSize], int reductionNum) {
	for (int i = 0; i < ArrSize; i++) {
		switch (dir) {
		case left:
		case right:
			arr[reductionNum][i] = temp[i];
			break;
		case down:
		case up:
			arr[i][reductionNum] = temp[i];
			break;
		}
	}
}


//是否开启键盘监听
void GameLayer::setKeyboardEnable(bool enable) {
	if (enable) {//开启
		auto listener = EventListenerKeyboard::create();
		listener->onKeyPressed = CC_CALLBACK_2(GameLayer::onKeyPressed, this);
		//事件分发
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	} else {//关闭
		_eventDispatcher->removeEventListenersForTarget(this);
	}
}

/*
	键盘监听
*/
void GameLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event*) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		dir = up;
		isInput = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		dir = down;
		isInput = true;
		break;

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		dir = left;
		isInput = true;
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		dir = right;
		isInput = true;
		break;
	}
}

/*
	数组清空
*/
void GameLayer::arrClearr() {
	for (size_t i = 0; i < ArrSize; i++) {
		temp[i] = 0;
	}
}

/*
	随机创建数字
*/
void GameLayer::randomCreateNum() {
	int x = rand() % ArrSize;
	int y = rand() % ArrSize;
	while (map[x][y] != 0) {
		x = rand() % ArrSize;
		y = rand() % ArrSize;
	}
	map[x][y] = rand() % 2 == 0 ? 2 : ArrSize;
}