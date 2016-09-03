#ifndef __MEMORY_CARD_LEVEL_H__
#define __MEMORY_CARD_LEVEL_H__
//���õĿ�Ƭ
#include <iostream>
#include "cocos2d.h"
#include "LevelData.h"
#include <vector>
#include "Card.h"
USING_NS_CC;

class MemoryCardLevel : public Layer {
	//�ؿ�����
	LevelData _levelData;
	//���õ�ǰ��Ƭ
	std::vector<std::vector<Card*>> _cardTable;
	//ʣ�࿨Ƭ
	int _unfinishedCard;
	//��ǰѡ��Ŀ�Ƭ
	Card* _selCardA;
	Card* _selCardB;
	//���ʱ�ص�
	std::function<void(CardData* cardA, CardData* cardB)> _pairCallback;
	//ȫ��ƥ���ص�
	std::function<void()> _completeCallBack;

public:
	//���캯��
	MemoryCardLevel();
	//��������
	virtual ~MemoryCardLevel();
	//cocos2dx����create����
	static MemoryCardLevel* create(LevelData levelData);
	//��ʼ������
	virtual bool initWithLevelData(LevelData levelData);
	//ע��ص�����
	void registerCallfunc(std::function<void(CardData* cardA, CardData* cardB)> pairCallBack, std::function<void()> completeCallBack);
protected:
	//��ʼ����Ƭ����
	void initCardLayout();
	//��ʼ�������¼��ĺ���
	void initTouchEvent();
};
#endif