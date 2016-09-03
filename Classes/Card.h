//��Ƭ��
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
	//״̬����
	CardState _state;
	//��Ƭ����
	CardData _cardData;
	//ע: CC_PROPERTY: �����Ƕ�����set��get��û��������������ʵ�֡�����set��get��ʵ�ֱȽϸ���
	//	  CC_SYNTHESIZE��ʵ����set��get�����������ʵ�ַ�ʽ
	//���������ͼ������
	CC_PROPERTY(Node*, _backImage, BackImage);
	CC_PROPERTY(Node*, _frontImage, FrontImage);
	//��Ƭ��ת������ͱ�����Ƿ�תһ���棬ֱ�����������ٷŴ���һ����ֱ����������������������ת������Ϊ����һ���ģ�����ת����
	//����a�淭ת��b��
	void flipFromTo(Node *a, Node *b, const std::function<void()> &callback = nullptr);
public:
	CREATE_FUNC(Card);
	//��Ƭ��ת�������ص�
	virtual void flipToBack(const std::function<void()> &callback = nullptr);
	virtual void flipToFront(const std::function<void()> &callback = nullptr);
	//��ȡ��Ƭ״̬
	virtual CardState getCardState()const;
	//���ÿ�Ƭ����
	virtual void setCardData(CardData data);
	//�õ���Ƭ����
	virtual CardData* getCardData();
};
#endif