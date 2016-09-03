#include "ScoreStrategy.h"
ScoreStrategy::ScoreStrategy():_continuous(0) {

}

void ScoreStrategy::execute(ScoreData* score, CardData* cardA, CardData* cardB) {
	//���ȶԿ�Ƭ�ı�Ž����ж�
	if (cardA->number == cardB->number) {
		//��Գɹ��� ��������ֵ�� �ۼ�������
		_continuous++;
		if (_continuous > score->maxContinuous) {
			score->maxContinuous = _continuous;
		}
		//����������������Ϸʱ��
		score->energy += 100;
		//���ӷ�����ȡ��������Ϊ��������������Խ�ߣ�����Խ��
		score->score += 100 * _continuous;
	}
	else {
		//ʧ�ܣ��۳���������
		//������ſ�Ƭ���ǵ�һ�η������򲻽��п۷ִ���
		if (cardA->flipCount == 0 && cardB->flipCount == 0) {

		}
		else { //���ǵ�һ�Σ���Ҫ���������۳�
			score->energy -= 100;
			//�����������
			_continuous = 0;
		}
	}
	//�ۼ�ÿ�ſ�Ƭ���Է����Ĵ���
	cardA->flipCount += 1;
	cardB->flipCount += 1;
}