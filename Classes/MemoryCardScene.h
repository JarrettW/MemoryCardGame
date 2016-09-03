#ifndef _MEMORY_CARD_SCENE_H_
#define _MEMORY_CARD_SCENE_H_
//������
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
	int _nowLevel;  //��ǰ�ؿ���
	int _allLevel;	//�ܵĹؿ���
	std::vector<LevelData> _levelDatalist;	//�������йؿ����б�

	LevelData _nowLeveldata;  //��ǰ�ؿ�����
	MemoryCardLevel* _nowLevelLayer; //��ǰ�ؿ�
	std::shared_ptr<ScoreStrategyBase> _scoreStrategy;  //���ֲ���
	ScoreData _scoreData;  //��������
	BackGround* _background;   //����
	EnergyBar* _energyBar;
	ScoreText* _scoreText; 
	//��ͣ��ť
	ui::Button* _pauseButton;
public:
	MemoryCardScene();
	virtual ~MemoryCardScene();
	static Scene* createScene(std::shared_ptr<ScoreStrategy> scoreStrategy);
	static MemoryCardScene* create(std::shared_ptr<ScoreStrategy> scoreStrategy);
	virtual bool initWithScoreStrategy(std::shared_ptr<ScoreStrategy> scoreStrategy);
	virtual void initLevelDataList();  //��ʼ���ؿ��б�
	virtual void initUI();   //��ʼ��UI����
	virtual void newGame();
	virtual void update(float t);
	virtual void nextLevel();  //��һ��
};
#endif