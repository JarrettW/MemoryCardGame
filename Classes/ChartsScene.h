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
	BackGround* _background;  //背景
	ui::Button* _backBut;  //返回

	Sprite* _newScoreTitle;  //最新分数标签图片
	ui::TextAtlas* _newScore;  //最新得分

	Sprite* _chartScoreTitle;  //排行分数标签图片
	std::vector<Node*> _charList;  //多个分数排行列表（可保存任意得分行数)
	//创建排行榜的每一行
	Node* createChart(int rank, int score); //1.当前的排名， 2.当前的分数
};
#endif // !_CHARTS_SCENE_H_