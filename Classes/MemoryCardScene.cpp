#include "MemoryCardScene.h"
#include "UserData.h"
#include "SceneMediator.h"
#include "SimpleAudioEngine.h"
MemoryCardScene::MemoryCardScene() :
	_nowLevelLayer(nullptr), _scoreData({}) {
	_scoreData.energy = 1000;   //初始化能量值为1000
}

MemoryCardScene::~MemoryCardScene() {
	this->unscheduleUpdate();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

Scene* MemoryCardScene::createScene(std::shared_ptr<ScoreStrategy> scoreStrategy) {
	auto scene = Scene::create();
	auto layer = MemoryCardScene::create(scoreStrategy);
	scene->addChild(layer);
	return scene;
}

MemoryCardScene* MemoryCardScene::create(std::shared_ptr<ScoreStrategy> scoreStrategy) {
	MemoryCardScene* scene = new MemoryCardScene();
	if (scene && scene->initWithScoreStrategy(scoreStrategy)) {
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return nullptr;
}


//启动函数
bool MemoryCardScene::initWithScoreStrategy(std::shared_ptr<ScoreStrategy> scoreStrategy) {
	if (Layer::init() == false)
		return false;

	_scoreStrategy = scoreStrategy;
	
	initLevelDataList();  //初始化关卡数据
	//背景音乐
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("pitpat.mp3");

	initUI();
	newGame();
	this->scheduleUpdate();
	return true;
}
void MemoryCardScene::initUI() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_background = BackGround::create();
	this->addChild(_background);

	//创建能量条和分数
	_energyBar = EnergyBar::create();
	_energyBar->setPosition(visibleSize.width / 2, visibleSize.height - 100.0f);
	this->addChild(_energyBar);

	_scoreText = ScoreText::create();
	_scoreText->setPosition(Vec2(visibleSize.width - 20, visibleSize.height - 33));
	_scoreText->setScale(0.75f);
	this->addChild(_scoreText);

	//创建暂停按钮
	_pauseButton = ui::Button::create("pause.png");
	_pauseButton->setAnchorPoint(Vec2(0, 1));
	_pauseButton->setPosition(Vec2(0, visibleSize.height));
	addChild(_pauseButton);
	//给按钮添加点击的事件操作
	_pauseButton->addClickEventListener([this](Ref* ref) {
		//当游戏进入暂停状态，update函数将停止执行
		this->unscheduleUpdate();
		//然后创建对话框
		auto pauseBox = Pause::create();
		pauseBox->registerCallback([this, pauseBox]() {
			//如果玩家点击继续按钮，则移除对话框
			pauseBox->removeFromParent();
			this->scheduleUpdate();
		}, []() {
			SceneMediator::getInstance()->gotoStartScene();
		});
		this->addChild(pauseBox);
	});
}

void MemoryCardScene::newGame() {
	//当前关卡的赋值
	_nowLevel = -1;
	//进入下一关
	nextLevel();
}

void MemoryCardScene::update(float t) {
	Layer::update(t);
	//在每帧减少一定的能量值
	_scoreData.energy -= this->_nowLeveldata.loss * t;
	//如果当前能量大于1000，那么就让他等于1000
	if (_scoreData.energy > 1000) {
		_scoreData.energy = 1000;
	}
	else if (_scoreData.energy < 0) {
		_scoreData.energy = 0;
	}
	_energyBar->updateView(_scoreData.energy);
	_scoreText->updateView(_scoreData.score);
	//如果当前能量值小于等于0，则代表当前游戏结束
	if (_scoreData.energy <= 0) {
		//游戏结束，记录当前分数
		UserDefault::getInstance()->setIntegerForKey(NEW_SCORE, _scoreData.score);
		//加入到排行榜中
		std::vector<int> scoreList; //所有分数
		scoreList.push_back(_scoreData.score);	//当前最新分数加入到该数组
		for (int i = 0; i < 5; ++i) {
			int score = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("%s%d", RANK_SCORE, i).c_str(), 0);
			scoreList.push_back(score);
		}
		//排序，从大到小
		std::sort(scoreList.begin(), scoreList.end(), [](int &a, int &b) { return a > b; });
		//把排序结果重新添加至排行榜数据中
		int rank = 0;
		for (auto i = scoreList.begin(); i < scoreList.end(); ++i) {
			UserDefault::getInstance()->setIntegerForKey(StringUtils::format("%s%d", RANK_SCORE, rank).c_str(), *i);
			rank++;
		}
		this->unscheduleUpdate();
		SceneMediator::getInstance()->gotoChartsScene();
	}
	if (_scoreData.energy < 600) {
		//如果小于600，它的音量将随着能量值的递减而增加， 根据比例进行计算
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume((600.0f - _scoreData.energy) / 600.0f);
	}
	else {
		//否则将心跳声的音量减少为0
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
	}
}

void MemoryCardScene::initLevelDataList() {
	//将csv文件加载到内存中
	auto str = FileUtils::getInstance()->getStringFromFile("myleveldata.csv");
	//创建数组保存每一行数据
	std::vector<std::string> rowList;
	//使用c函数对字符串进行解析，参数（被切割的字符串， 切割符）
	char *row = strtok((char*)str.c_str(), "\n");
	//while循环持续切割，切割完毕row为空
	while (row) {
		//切割成功存入数组
		rowList.push_back(row);
		//并且继续切割, 第二次切割不需要传入字符串
		row = strtok(nullptr, "\n");  
	}
	//遍历所有行进行列的切割
	for (auto row = rowList.begin(); row != rowList.end(); ++row) {
		//直接切割3次，分别取出
		//当前的行数
		char *rows = strtok((char*)row->c_str(), ",");
		//当前的列数
		char *columns = strtok(nullptr, ",");
		//能量损耗值
		char *loss = strtok(nullptr, ",");
		//对取出的数据进行判断,如果三者任一个为空则跳过
		if (rows == nullptr || columns == nullptr || loss == nullptr) {
			continue;
		}
		//创建LevelData对象，进行赋值
		LevelData level;
		level.rows = atoi(rows);
		level.columns = atoi(columns);
		level.loss = atoi(loss);
		//判断行数*列数不能小于等于0，行*列必须被2整除，损耗值不可以是负数，否则关卡也没意义
		if (level.rows * level.columns <= 0 || (level.columns * level.rows) % 2 != 0 || level.loss < 0) {
			continue;
		}
		//将处理完的关卡数据加入到数组中
		_levelDatalist.push_back(level);
	}
	_allLevel = static_cast<int>(_levelDatalist.size());  //赋值总关卡的数量
}

void MemoryCardScene::nextLevel() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//每进入新的一关_nowLevel+1
	_nowLevel++;
	if (_nowLevel >= _allLevel) {
		_nowLevel = 0;
	}
	//直接从已经加载完的关卡数组中取出即可
	_nowLeveldata = _levelDatalist[_nowLevel];

	//实例化一个关卡
	auto level = _nowLevelLayer = MemoryCardLevel::create(_nowLeveldata);
	auto levelSize = level->getContentSize();
	level->setAnchorPoint(Vec2(0.5, 0.5));
	level->ignoreAnchorPointForPosition(false);
	level->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	//设置一缩放比例
	auto scale = visibleSize.width / (levelSize.width + 100);
	//将关卡向下移动一定的像素
	level->setPositionY(level->getPositionY() - 80 * scale);

	level->setScale(2);
	level->setOpacity(0);
	this->addChild(level);
	//添加一个特效，进入新关卡时
	level->runAction(Spawn::create(FadeIn::create(0.25), ScaleTo::create(0.25, scale), NULL));
	_nowLevelLayer = level;  //赋值当前关卡

	level->registerCallfunc([this](CardData* cardA, CardData* cardB) {
		_scoreStrategy->execute(&_scoreData, cardA, cardB);
		log("score:%d, energy:%d, macCountinuous:%d", _scoreData.score, _scoreData.energy, _scoreData.maxContinuous);
	}, [this]() {
		log("complete\n");
		//当每关结束时，进行移除
		_nowLevelLayer->removeFromParent();
		nextLevel();
	});

}