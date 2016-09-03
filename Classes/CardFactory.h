#ifndef __CARD_FACTORY_H__
#define __CARD_FACTORY_H__
#include <stdio.h>
//#include "Card.h"
#include "ICard.h"

class CardFactory
{
public:
	//创建卡片
	virtual ICard* createCard(int backId, int number); //1.卡片背面ID，2.正面显示的编号
};
#endif // !__CARD_FACTORY_H__