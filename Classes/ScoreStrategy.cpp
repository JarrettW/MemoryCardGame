#include "ScoreStrategy.h"
ScoreStrategy::ScoreStrategy():_continuous(0) {

}

void ScoreStrategy::execute(ScoreData* score, CardData* cardA, CardData* cardB) {
	//首先对卡片的编号进行判断
	if (cardA->number == cardB->number) {
		//配对成功， 增加能量值， 累加连击数
		_continuous++;
		if (_continuous > score->maxContinuous) {
			score->maxContinuous = _continuous;
		}
		//增加能量，延续游戏时间
		score->energy += 100;
		//增加分数，取连击数作为倍数，即连击数越高，分数越高
		score->score += 100 * _continuous;
	}
	else {
		//失败，扣除部分能量
		//如果两张卡片都是第一次翻开，则不进行扣分处理
		if (cardA->flipCount == 0 && cardB->flipCount == 0) {

		}
		else { //不是第一次，则要进行能量扣除
			score->energy -= 100;
			//并清除连击数
			_continuous = 0;
		}
	}
	//累加每张卡片尝试翻开的次数
	cardA->flipCount += 1;
	cardB->flipCount += 1;
}