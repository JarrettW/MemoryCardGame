#ifndef _SCORE_STRATEGY_H_
#define _SCORE_STRATEGY_H_
#include "stdio.h"
#include "ScoreStrategyBase.h"
//���ֲ����ࣨ���ࣩ
class ScoreStrategy : public ScoreStrategyBase {
private:
	//������������������ֵ���ڵ�ǰ������������򸲸�֮
	int _continuous;
public:
	ScoreStrategy();
	virtual void execute(ScoreData *score, CardData* cardA, CardData* cardB);
};
#endif // !_SCORE_STRATEGY_H_