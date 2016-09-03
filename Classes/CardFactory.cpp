#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Card.h"
#include "CardFactory.h"
USING_NS_CC;

//卡片背面ID， 正面显示的编号
ICard* CardFactory::createCard(int backId, int number) {
	//卡片大小。 由于资源大小256,所以定义同样256？
	const int cardSize = 256;
	//创建背景，根据0-7的ID来创建，卡片是以横向纵向排列，一共8张
	//首先验证ID是否有效
	if (backId < 0 || backId >= 8)
		backId = 0;
	//卡片一共有两列
	//取横向的位置，只需要求模2, 再乘以卡片大小
	int x = (backId % 2) * cardSize;
	//取纵向的位置，只需要除以2， 再乘以卡片大小
	int y = (backId / 2) * cardSize;

	//创建背面
	auto backImage = Sprite::create("card_back.png", Rect(x, y, cardSize, cardSize));

	//创建正面，正面由底色纹理和编号两部分组成
	auto frontImage = Node::create();
	frontImage->setContentSize(Size(cardSize, cardSize));
	frontImage->addChild(Sprite::create("card_front.png"));

	auto numberText = ui::TextAtlas::create(StringUtils::format("%d", number), "card_number.png", 140, 140, "0");

	frontImage->addChild(numberText);

	//创建卡片的数据对象
	CardData data = {};
	data.number = number; //复制卡片的编号

	//创建卡片
	auto card = Card::create();
	card->setBackImage(backImage);  //设置背景
	card->setFrontImage(frontImage); //设置正面
	card->setCardData(data);
	card->setContentSize(backImage->getContentSize());
	return card;
}