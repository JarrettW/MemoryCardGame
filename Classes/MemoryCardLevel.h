#ifndef __MEMORY_CARD_LEVEL_H__
#define __MEMORY_CARD_LEVEL_H__
//布置的卡片
#include <iostream>
#include "cocos2d.h"
#include "LevelData.h"
#include <vector>
#include "Card.h"
USING_NS_CC;

class MemoryCardLevel : public Layer {
	//关卡数据
	LevelData _levelData;
	//放置当前卡片
	std::vector<std::vector<Card*>> _cardTable;
	//剩余卡片
	int _unfinishedCard;
	//当前选择的卡片
	Card* _selCardA;
	Card* _selCardB;
	//配对时回调
	std::function<void(CardData* cardA, CardData* cardB)> _pairCallback;
	//全部匹配后回调
	std::function<void()> _completeCallBack;

public:
	//构造函数
	MemoryCardLevel();
	//析构函数
	virtual ~MemoryCardLevel();
	//cocos2dx风格的create函数
	static MemoryCardLevel* create(LevelData levelData);
	//初始化函数
	virtual bool initWithLevelData(LevelData levelData);
	//注册回调函数
	void registerCallfunc(std::function<void(CardData* cardA, CardData* cardB)> pairCallBack, std::function<void()> completeCallBack);
protected:
	//初始化卡片布局
	void initCardLayout();
	//初始化触摸事件的函数
	void initTouchEvent();
};
#endif