#include "Card.h"
Card::Card():_state(CardState::BACK), _backImage(nullptr), _frontImage(nullptr){
	this->setCascadeColorEnabled(true);  //设置是否启用层叠颜色。随父类，传递了颜色设置的作用
}

Card::~Card() {
	this->removeAllChildren();
}

Node* Card::getBackImage() {
	return _backImage;
}

Node* Card::getFrontImage() {
	return _frontImage;
}

void Card::setFrontImage(Node* var) {
	if (nullptr == var)
		return;
	//如果已存在，则移除
	if (nullptr != _frontImage)
		_frontImage->removeFromParent();
	//重新设置正面图片
	_frontImage = var;
	//随父类，传递了透明设置的作用
	_frontImage->setCascadeOpacityEnabled(true);
	//正面默认显示隐藏
	_frontImage->setVisible(false);
	addChild(_frontImage);
}

void Card::setBackImage(Node* var) {
	if (nullptr == var)
		return;
	//如果已存在，则移除
	if (nullptr != _backImage)
		_backImage->removeFromParent();
	//重新设置正面图片
	_backImage = var;
	//随父类，传递了透明设置的作用
	_backImage->setCascadeOpacityEnabled(true);
	//正面默认显示隐藏
	//_backImage->setVisible(true);
	addChild(_backImage);
}
//从a面翻转到b面
void Card::flipFromTo(Node* a, Node* b, const std::function<void()> &callback) {
	if (a == nullptr || b == nullptr)
		return;
	//初始化属性
	a->stopAllActions();
	b->stopAllActions();

	a->setVisible(true);
	a->setScale(1.0f);

	b->setVisible(false);
	b->setScale(0);

	//B放大,翻出B
	auto flipB = [a, b, callback]() {
		a->setVisible(false);
		b->setVisible(true);

		CallFunc *func = nullptr;
		if (callback != nullptr) {
			func = CallFunc::create(callback);
		}
		b->runAction(Sequence::create(ScaleTo::create(0.125f, 1.0f, 1.0f), func, NULL));
	};
	//A翻入完成后，回调B
	a->runAction(Sequence::create(ScaleTo::create(0.125f, 0, 1.0f), CallFunc::create(flipB), NULL));
}
//翻转到背面
void Card::flipToBack(const std::function<void()> &callback) {
	//如果卡片状态是背面，则不必再翻转
	if (_state == CardState::BACK)
		return;
	//从正面图片转换到背面
	flipFromTo(_frontImage, _backImage, callback);
	//设置卡片状态为背面
	_state = CardState::BACK;
}
//翻转到正面
void Card::flipToFront(const std::function<void()> &callback) {
	if (_state == CardState::FRONT)
		return;
	flipFromTo(_backImage, _frontImage, callback);
	_state = CardState::FRONT;
}
//获取卡片状态
CardState Card::getCardState()const{
	return _state;
}
//设置卡片数据
void Card::setCardData(CardData data) {
	_cardData = data;
}
//得到卡片数据
CardData* Card::getCardData(){
	return &_cardData;
}
