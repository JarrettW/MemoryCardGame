#include "SceneMediator.h"
#include "StartScene.h"
#include "ScoreStrategy.h"
#include "MemoryCardScene.h"
#include "ChartsScene.h"

SceneMediator* SceneMediator::_instance = nullptr;

SceneMediator* SceneMediator::getInstance() {
	if (_instance == nullptr) {
		_instance = new SceneMediator();
	}
	return _instance;
}

void SceneMediator::gotoStartScene() {
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(0.5, scene));
}

void SceneMediator::gotoGameScene() {
	std::shared_ptr<ScoreStrategy> scoreStrategy(new ScoreStrategy());
	auto scene = MemoryCardScene::createScene(scoreStrategy);
	Director::getInstance()->replaceScene(scene);
}

void SceneMediator::gotoChartsScene() {
	auto scene = ChartsScene::createScene();
	Director::getInstance()->replaceScene(scene);
}