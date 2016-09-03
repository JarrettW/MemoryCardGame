#ifndef _CHARTS_SCENE_H_
#define _CHARTS_SCENE_H_
#include <stdio.h>
#include "cocos2d.h"
#include "BackGround.h"
#include "ui\CocosGUI.h"
#include <vector>
USING_NS_CC;
class ChartsScene :public Layer {
public:
	ChartsScene();
	~ChartsScene();
	CREATE_FUNC(ChartsScene);
	virtual bool init();
	static Scene* createScene();
protected:
	BackGround* _background;  //����
	ui::Button* _backBut;  //����

	Sprite* _newScoreTitle;  //���·�����ǩͼƬ
	ui::TextAtlas* _newScore;  //���µ÷�

	Sprite* _chartScoreTitle;  //���з�����ǩͼƬ
	std::vector<Node*> _charList;  //������������б��ɱ�������÷�����)
	//�������а��ÿһ��
	Node* createChart(int rank, int score); //1.��ǰ�������� 2.��ǰ�ķ���
};
#endif // !_CHARTS_SCENE_H_