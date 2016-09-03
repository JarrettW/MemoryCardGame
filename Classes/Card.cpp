#include "Card.h"
Card::Card():_state(CardState::BACK), _backImage(nullptr), _frontImage(nullptr){
	this->setCascadeColorEnabled(true);  //�����Ƿ����ò����ɫ���游�࣬��������ɫ���õ�����
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
	//����Ѵ��ڣ����Ƴ�
	if (nullptr != _frontImage)
		_frontImage->removeFromParent();
	//������������ͼƬ
	_frontImage = var;
	//�游�࣬������͸�����õ�����
	_frontImage->setCascadeOpacityEnabled(true);
	//����Ĭ����ʾ����
	_frontImage->setVisible(false);
	addChild(_frontImage);
}

void Card::setBackImage(Node* var) {
	if (nullptr == var)
		return;
	//����Ѵ��ڣ����Ƴ�
	if (nullptr != _backImage)
		_backImage->removeFromParent();
	//������������ͼƬ
	_backImage = var;
	//�游�࣬������͸�����õ�����
	_backImage->setCascadeOpacityEnabled(true);
	//����Ĭ����ʾ����
	//_backImage->setVisible(true);
	addChild(_backImage);
}
//��a�淭ת��b��
void Card::flipFromTo(Node* a, Node* b, const std::function<void()> &callback) {
	if (a == nullptr || b == nullptr)
		return;
	//��ʼ������
	a->stopAllActions();
	b->stopAllActions();

	a->setVisible(true);
	a->setScale(1.0f);

	b->setVisible(false);
	b->setScale(0);

	//B�Ŵ�,����B
	auto flipB = [a, b, callback]() {
		a->setVisible(false);
		b->setVisible(true);

		CallFunc *func = nullptr;
		if (callback != nullptr) {
			func = CallFunc::create(callback);
		}
		b->runAction(Sequence::create(ScaleTo::create(0.125f, 1.0f, 1.0f), func, NULL));
	};
	//A������ɺ󣬻ص�B
	a->runAction(Sequence::create(ScaleTo::create(0.125f, 0, 1.0f), CallFunc::create(flipB), NULL));
}
//��ת������
void Card::flipToBack(const std::function<void()> &callback) {
	//�����Ƭ״̬�Ǳ��棬�򲻱��ٷ�ת
	if (_state == CardState::BACK)
		return;
	//������ͼƬת��������
	flipFromTo(_frontImage, _backImage, callback);
	//���ÿ�Ƭ״̬Ϊ����
	_state = CardState::BACK;
}
//��ת������
void Card::flipToFront(const std::function<void()> &callback) {
	if (_state == CardState::FRONT)
		return;
	flipFromTo(_backImage, _frontImage, callback);
	_state = CardState::FRONT;
}
//��ȡ��Ƭ״̬
CardState Card::getCardState()const{
	return _state;
}
//���ÿ�Ƭ����
void Card::setCardData(CardData data) {
	_cardData = data;
}
//�õ���Ƭ����
CardData* Card::getCardData(){
	return &_cardData;
}
