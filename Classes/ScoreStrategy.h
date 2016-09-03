#ifndef _SCORE_STRATEGY_H_
#define _SCORE_STRATEGY_H_
#include "stdio.h"
#include "ScoreStrategyBase.h"
//积分策略类（子类）
class ScoreStrategy : public ScoreStrategyBase {
private:
	//最大连击数，如果它的值大于当前最大连击数，则覆盖之
	int _continuous;
public:
	ScoreStrategy();
	virtual void execute(ScoreData *score, CardData* cardA, CardData* cardB);
};
#endif // !_SCORE_STRATEGY_H_