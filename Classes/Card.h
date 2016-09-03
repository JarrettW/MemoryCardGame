//卡片类
#ifndef __CARD_H__
#define __CARD_H__
#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
#include "ICard.h"

class Card:public ICard, public Node{
	Card();
	virtual ~Card();
protected:
	//状态属性
	CardState _state;
	//卡片数据
	CardData _cardData;
	//注: CC_PROPERTY: 仅仅是定义了set和get并没有完成这个函数的实现。可能set和get的实现比较复杂
	//	  CC_SYNTHESIZE：实现了set和get就是最基本的实现方式
	//正反两面的图像属性
	CC_PROPERTY(Node*, _backImage, BackImage);
	CC_PROPERTY(Node*, _frontImage, FrontImage);
	//卡片翻转到正面和背面就是翻转一个面，直到看不见，再放大另一个面直到正常比例。下面两个翻转函数行为都是一样的，处理翻转函数
	//即从a面翻转到b面
	void flipFromTo(Node *a, Node *b, const std::function<void()> &callback = nullptr);
public:
	CREATE_FUNC(Card);
	//卡片翻转函数，回调
	virtual void flipToBack(const std::function<void()> &callback = nullptr);
	virtual void flipToFront(const std::function<void()> &callback = nullptr);
	//获取卡片状态
	virtual CardState getCardState()const;
	//设置卡片数据
	virtual void setCardData(CardData data);
	//得到卡片数据
	virtual CardData* getCardData();
};
#endif