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
	//��������Ϊƽ��
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	//���ñ�������
	_background->getTexture()->setTexParameters(tp);
	//����������ʾ����
	auto viewSize = Director::getInstance()->getVisibleSize();
	auto rect = Rect(0, 0, viewSize.width, viewSize.height);
	_background->setTextureRect(rect);

	this->addChild(_background);  //�������������ʾ�б�
	this->scheduleUpdate();
	return true;
}

void BackGround::update(float t) {
	Node::update(t);

	auto rect = _background->getTextureRect();
	rect.origin.x += 100.0f * t;  //t���ֲ��������ʱ��һ֡����һ֡��ʱ��
	rect.origin.y += 100.0f * t;
	_background->setTextureRect(rect);
}