
#ifndef _SCORE_STRATEGY_BASE_H__
#define _SCORE_STRATEGY_BASE_H__
#include "ScoreData.h"
#include "CardData.h"
//积分策略基类
class ScoreStrategyBase {
public:
	virtual void execute(ScoreData *score, CardData* cardA, CardData* cardB) = 0;
};
#endif