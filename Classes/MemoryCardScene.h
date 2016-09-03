#ifndef _MEMORY_CARD_SCENE_H_
#define _MEMORY_CARD_SCENE_H_
//场景类
#include <stdio.h>
#include "cocos2d.h"
#include "MemoryCardLevel.h"
#include <vector>
#include "ScoreStrategy.h"
#include <memory>
#include "BackGround.h"
#include "EnergyBar.h"
#include "ScoreText.h"
#include "Pause.h"
USING_NS_CC;
class MemoryCardScene : public Layer {
protected:
	int _nowLevel;  //当前关卡数
	int _allLevel;	//总的关卡数
	std::vector<LevelData> _levelDatalist;	//保存所有关卡的列表

	LevelData _nowLeveldata;  //当前关卡数据
	MemoryCardLevel* _nowLevelLayer; //当前关卡
	std::shared_ptr<ScoreStrategyBase> _scoreStrategy;  //积分策略
	ScoreData _scoreData;  //积分数据
	BackGround* _background;   //背景
	EnergyBar* _energyBar;
	ScoreText* _scoreText; 
	//暂停按钮
	ui::Button* _pauseButton;
public:
	MemoryCardScene();
	virtual ~MemoryCardScene();
	static Scene* createScene(std::shared_ptr<ScoreStrategy> scoreStrategy);
	static MemoryCardScene* create(std::shared_ptr<ScoreStrategy> scoreStrategy);
	virtual bool initWithScoreStrategy(std::shared_ptr<ScoreStrategy> scoreStrategy);
	virtual void initLevelDataList();  //初始化关卡列表
	virtual void initUI();   //初始化UI函数
	virtual void newGame();
	virtual void update(float t);
	virtual void nextLevel();  //下一关
};
#endif