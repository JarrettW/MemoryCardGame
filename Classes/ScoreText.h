#ifndef _SCORE_TEXT_H_
#define _SCORE_TEXT_H_
#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class ScoreText : public Node {
protected:
	//ʹ��CocosGUI�⣬�����ı�����
	ui::TextAtlas* _text;
	//����
	int _score;
public:
	ScoreText();
	CREATE_FUNC(ScoreText);
	virtual bool init();
	void updateView(int value);
};
#endif // !_SCORE_TEXT_H