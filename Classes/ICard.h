//��Ƭ�ӿ� Interface Card
#ifndef __ICARD_H__
#define __ICARD_H__
#include <functional>
#include "CardState.h"
#include "CardData.h"

class ICard{
public:
	//��Ƭ��ת�������ص�
	virtual void flipToBack(const std::function<void()> &callback = nullptr) = 0;
	virtual void flipToFront(const std::function<void()> &callback = nullptr) = 0;
	//��ȡ��Ƭ״̬
	virtual CardState getCardState()const = 0;
	//���ÿ�Ƭ����
	virtual void setCardData(CardData data) = 0;
	//�õ���Ƭ����
	virtual CardData* getCardData() = 0;

};
#endif // !__ICARD_H__