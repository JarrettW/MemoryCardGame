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
	//ע��ص�������������1.���ڼ�����Ϸʱ�ص� 2.���ڽ�����Ϸʱ�ص�
	virtual void registerCallback(std::function<void()> continuecCallback, std::function<void()> endGameCallback);
protected:
	std::function<void()> _continueCallback;
	std::function<void()> _endGameCallback;
};
#endif