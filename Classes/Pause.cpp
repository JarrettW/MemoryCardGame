#include "Pause.h"
//导入GUI库
#include "ui/CocosGUI.h"

Pause::Pause() {

}
Pause::~Pause() {

}

bool Pause::init() {
	if (Layer::init() == false) {
		return false;
	}
	//暂停对话框的布局

	auto viewSize = Director::getInstance()->getVisibleSize();
	//create background
	auto background = LayerColor::create(Color4B(0, 0, 0, 200));
	auto box = ui::Scale9Sprite::create("message_box.png"); //九宫格
	auto continueBut = ui::Button::create("continue.png");
	auto endGameBut = ui::Button::create("end_game.png");

	box->setContentSize(Size(450, 300));  //设置黄色对话框的大小
	box->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2));

	continueBut->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 + 75));
	endGameBut->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 - 75));

	//当前Layer大小
	this->setContentSize(viewSize);

	this->addChild(background);
	this->addChild(box);
	this->addChild(continueBut);
	this->addChild(endGameBut);

	//注册两个按钮的单击事件
	continueBut->addClickEventListener([this](Ref* ref) {
		//回调函数验证，不为空，则回调
		if (_continueCallback != nullptr) {
			_continueCallback();
		}
	});
	endGameBut->addClickEventListener([this](Ref* ref) {
		if (_endGameCallback != nullptr) {
			_endGameCallback();
		}
	});
	//由于暂停对话框要挡住当前对话框之下的所有对话框的触摸事件，所以在背景对象上捕获
	//触摸操作，从而拦截触摸事件向下的传递
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch *t, Event *e) { return true; };
	listener->setSwallowTouches(true);

	background->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, background);

	return true;
}
//在使用的时候，传入回调参数
void Pause::registerCallback(std::function<void()> continueCallback, std::function<void()> endGameCallback) {
	_continueCallback = continueCallback;
	_endGameCallback = endGameCallback;
}