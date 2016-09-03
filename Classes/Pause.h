#ifndef _PAUSE_H_
#define _PAUSE_H_
#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
class Pause : public Layer {
public:
	Pause();
	virtual ~Pause();
	CREATE_FUNC(Pause);
	virtual bool init();
	//注册回调函数，参数：1.用于继续游戏时回调 2.用于结束游戏时回调
	virtual void registerCallback(std::function<void()> continuecCallback, std::function<void()> endGameCallback);
protected:
	std::function<void()> _continueCallback;
	std::function<void()> _endGameCallback;
};
#endif