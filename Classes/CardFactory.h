#ifndef __CARD_FACTORY_H__
#define __CARD_FACTORY_H__
#include <stdio.h>
//#include "Card.h"
#include "ICard.h"

class CardFactory
{
public:
	//������Ƭ
	virtual ICard* createCard(int backId, int number); //1.��Ƭ����ID��2.������ʾ�ı��
};
#endif // !__CARD_FACTORY_H__