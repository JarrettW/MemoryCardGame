#include "EnergyBar.h"

EnergyBar::EnergyBar() :
	_percentage(0) {

}
EnergyBar::~EnergyBar() {
	//this->unscheduleUpdate();
}

bool EnergyBar::init() {
	if (Node::init() == false)
		return false;

	_progress = ProgressTimer::create(Sprite::create("energy_front.png"));
	//设置进度条类型
	_progress->setType(cocos2d::ProgressTimer::Type::BAR);
	//设置中点，设置为左边
	_progress->setMidpoint(Vec2(0, 0));
	//设置可变区域
	_progress->setBarChangeRate(Vec2(1, 0));   //x轴可以变化，y轴不可变化

	//创建背景
	_background = Sprite::create("energy_back.png");

	this->addChild(_background);
	this->addChild(_progress);

	this->setContentSize(_background->getContentSize());

	updateView(1000.0f);
	return true;
}

void EnergyBar::updateView(int value) {  //value：能量值
	auto percentage = value / 1000.0f * 100.0f;
	if (abs(percentage - _percentage) > 1.0f) {
		_progress->runAction(EaseBackOut::create(ProgressTo::create(0.25f, percentage)));
		_percentage = percentage;
	}
}