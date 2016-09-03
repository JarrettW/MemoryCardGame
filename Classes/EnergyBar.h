#ifndef _ENERGY_H_
#define _ENERGY_H_
#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
class EnergyBar : public Node {
public:
	EnergyBar();
	virtual ~EnergyBar();
	CREATE_FUNC(EnergyBar);
	virtual bool init();
	void updateView(int value); //����ˢ�µ�ǰ����ͼ
protected:
	//������������
	ProgressTimer* _progress;
	//�������ķ���
	Sprite* _background;
	//�ٷֱ�
	float _percentage;
};
#endif