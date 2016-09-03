#include "Pause.h"
//����GUI��
#include "ui/CocosGUI.h"

Pause::Pause() {

}
Pause::~Pause() {

}

bool Pause::init() {
	if (Layer::init() == false) {
		return false;
	}
	//��ͣ�Ի���Ĳ���

	auto viewSize = Director::getInstance()->getVisibleSize();
	//create background
	auto background = LayerColor::create(Color4B(0, 0, 0, 200));
	auto box = ui::Scale9Sprite::create("message_box.png"); //�Ź���
	auto continueBut = ui::Button::create("continue.png");
	auto endGameBut = ui::Button::create("end_game.png");

	box->setContentSize(Size(450, 300));  //���û�ɫ�Ի���Ĵ�С
	box->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2));

	continueBut->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 + 75));
	endGameBut->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 - 75));

	//��ǰLayer��С
	this->setContentSize(viewSize);

	this->addChild(background);
	this->addChild(box);
	this->addChild(continueBut);
	this->addChild(endGameBut);

	//ע��������ť�ĵ����¼�
	continueBut->addClickEventListener([this](Ref* ref) {
		//�ص�������֤����Ϊ�գ���ص�
		if (_continueCallback != nullptr) {
			_continueCallback();
		}
	});
	endGameBut->addClickEventListener([this](Ref* ref) {
		if (_endGameCallback != nullptr) {
			_endGameCallback();
		}
	});
	//������ͣ�Ի���Ҫ��ס��ǰ�Ի���֮�µ����жԻ���Ĵ����¼��������ڱ��������ϲ���
	//�����������Ӷ����ش����¼����µĴ���
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch *t, Event *e) { return true; };
	listener->setSwallowTouches(true);

	background->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, background);

	return true;
}
//��ʹ�õ�ʱ�򣬴���ص�����
void Pause::registerCallback(std::function<void()> continueCallback, std::function<void()> endGameCallback) {
	_continueCallback = continueCallback;
	_endGameCallback = endGameCallback;
}