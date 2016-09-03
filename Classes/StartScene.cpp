#include "StartScene.h"
#include "SceneMediator.h"

StartScene::StartScene():
	_background(nullptr),
	_logo(nullptr),
	_startBtn(nullptr),
	_chartBtn(nullptr),
	_exitBtn(nullptr){

}

StartScene::~StartScene() {

}

Scene* StartScene::createScene() {
	auto layer = StartScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init() {
	if (Layer::init() == false) {
		return false;
	}
	auto viewSize = Director::getInstance()->getVisibleSize();

	_background = BackGround::create();
	//_background->setAnchorPoint(Vec2(0, 0));
	//_background->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2));

	_logo = Sprite::create("logo.png");
	_logo->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 + 220));

	_startBtn = ui::Button::create("start.png");
	_startBtn->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 - 100));

	_chartBtn = ui::Button::create("charts.png");
	_chartBtn->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 - 220));

	_exitBtn = ui::Button::create("end_game.png");
	_exitBtn->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 - 340));

	this->addChild(_background);
	this->addChild(_logo);
	this->addChild(_startBtn);
	this->addChild(_chartBtn);
	this->addChild(_exitBtn);

	_startBtn->addClickEventListener([this](Ref* ref) {
		SceneMediator::getInstance()->gotoGameScene();
	});

	_chartBtn->addClickEventListener([this](Ref* ref) {
		SceneMediator::getInstance()->gotoChartsScene();
	});
	
	_exitBtn->addClickEventListener([this](Ref* ref) {
		Director::getInstance()->end();
	});


	return true;
}