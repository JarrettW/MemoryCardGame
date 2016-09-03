#include "ScoreText.h"
ScoreText::ScoreText(): _score(0){

}

bool ScoreText::init() {
	if (Node::init() == false) {
		return false;
	}
	_text = ui::TextAtlas::create("0", "number.png", 63, 83, "0");
	_text->setAnchorPoint(Vec2(1.0, 0.5));
	this->addChild(_text);
	return true;
}

void ScoreText::updateView(int value) {
	//判断传入的参数是否与当前分数相等
	if (_score == value)
		return;
	_score = value;
	_text->setString(StringUtils::format("%d", value));
	
	auto effect = _text->clone();
	effect->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.25, 2),
													 FadeOut::create(0.25),NULL),
													 CallFunc::create([effect]() {
		effect->removeFromParent();
	}), NULL));
	this->addChild(effect);
}