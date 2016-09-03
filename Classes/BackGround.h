#ifndef _BACK_GROUND_H_
#define _BACK_GROUND_H_
#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
class BackGround :public Node {
public:
	BackGround();
	virtual ~BackGround();
	CREATE_FUNC(BackGround);
	virtual bool init();
	//ÈÃ±³¾°¶¯ÆðÀ´
	virtual void update(float t);
protected:
	Sprite* _background;
};
#endif