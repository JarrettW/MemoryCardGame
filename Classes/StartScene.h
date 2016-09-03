#ifndef _START_SCENE_H_
#define _START_SCENE_H_
#include <stdio.h>
#include "cocos2d.h"
#include "BackGround.h"
#include "ui/cocosGUI.h"
USING_NS_CC;
class StartScene : public Layer {
public:
	StartScene();
	~StartScene();
	CREATE_FUNC(StartScene);
	static Scene* createScene();
	virtual bool init();
protected:
	BackGround* _background;
	Sprite* _logo;
	ui::Button* _startBtn;
	ui::Button* _chartBtn;
	ui::Button* _exitBtn;
};
#endif // !_START_SCENE_H_