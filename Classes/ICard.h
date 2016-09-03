//卡片接口 Interface Card
#ifndef __ICARD_H__
#define __ICARD_H__
#include <functional>
#include "CardState.h"
#include "CardData.h"

class ICard{
public:
	//卡片翻转函数，回调
	virtual void flipToBack(const std::function<void()> &callback = nullptr) = 0;
	virtual void flipToFront(const std::function<void()> &callback = nullptr) = 0;
	//获取卡片状态
	virtual CardState getCardState()const = 0;
	//设置卡片数据
	virtual void setCardData(CardData data) = 0;
	//得到卡片数据
	virtual CardData* getCardData() = 0;

};
#endif // !__ICARD_H__