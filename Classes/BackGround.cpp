#include "BackGround.h"
BackGround::BackGround(){

}

BackGround::~BackGround() {
	this->unscheduleUpdate();
}

bool BackGround::init() {
	if (Node::init() == false)
		return false;
	_background = Sprite::create("background.png");
	_background->setAnchorPoint(Vec2(0, 0));
	//将背景设为平铺
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	//设置背景纹理
	_background->getTexture()->setTexParameters(tp);
	//设置纹理显示区域
	auto viewSize = Director::getInstance()->getVisibleSize();
	auto rect = Rect(0, 0, viewSize.width, viewSize.height);
	_background->setTextureRect(rect);

	this->addChild(_background);  //将纹理添加至显示列表
	this->scheduleUpdate();
	return true;
}

void BackGround::update(float t) {
	Node::update(t);

	auto rect = _background->getTextureRect();
	rect.origin.x += 100.0f * t;  //t这种参数代表的时上一帧与下一帧的时差
	rect.origin.y += 100.0f * t;
	_background->setTextureRect(rect);
}