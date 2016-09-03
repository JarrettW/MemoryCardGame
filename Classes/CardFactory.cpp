#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Card.h"
#include "CardFactory.h"
USING_NS_CC;

//��Ƭ����ID�� ������ʾ�ı��
ICard* CardFactory::createCard(int backId, int number) {
	//��Ƭ��С�� ������Դ��С256,���Զ���ͬ��256��
	const int cardSize = 256;
	//��������������0-7��ID����������Ƭ���Ժ����������У�һ��8��
	//������֤ID�Ƿ���Ч
	if (backId < 0 || backId >= 8)
		backId = 0;
	//��Ƭһ��������
	//ȡ�����λ�ã�ֻ��Ҫ��ģ2, �ٳ��Կ�Ƭ��С
	int x = (backId % 2) * cardSize;
	//ȡ�����λ�ã�ֻ��Ҫ����2�� �ٳ��Կ�Ƭ��С
	int y = (backId / 2) * cardSize;

	//��������
	auto backImage = Sprite::create("card_back.png", Rect(x, y, cardSize, cardSize));

	//�������棬�����ɵ�ɫ����ͱ�����������
	auto frontImage = Node::create();
	frontImage->setContentSize(Size(cardSize, cardSize));
	frontImage->addChild(Sprite::create("card_front.png"));

	auto numberText = ui::TextAtlas::create(StringUtils::format("%d", number), "card_number.png", 140, 140, "0");

	frontImage->addChild(numberText);

	//������Ƭ�����ݶ���
	CardData data = {};
	data.number = number; //���ƿ�Ƭ�ı��

	//������Ƭ
	auto card = Card::create();
	card->setBackImage(backImage);  //���ñ���
	card->setFrontImage(frontImage); //��������
	card->setCardData(data);
	card->setContentSize(backImage->getContentSize());
	return card;
}