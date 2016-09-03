#ifndef _SCENE_MEDIATOR_H_
#define _SCENE_MEDIATOR_H_
#include "cocos2d.h"
USING_NS_CC;

class SceneMediator {
protected:
	static SceneMediator* _instance;
public:
	static SceneMediator* getInstance();
	virtual void gotoStartScene();
	virtual void gotoGameScene();
	virtual void gotoChartsScene();
};
#endif // !_SCENE_MEDIATOR_H_