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
	void updateView(int value); //用于刷新当前的视图
protected:
	//进度条的正面
	ProgressTimer* _progress;
	//进度条的反面
	Sprite* _background;
	//百分比
	float _percentage;
};
#endif