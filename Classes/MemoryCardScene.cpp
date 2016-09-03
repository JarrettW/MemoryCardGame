#include "MemoryCardScene.h"
#include "UserData.h"
#include "SceneMediator.h"
#include "SimpleAudioEngine.h"
MemoryCardScene::MemoryCardScene() :
	_nowLevelLayer(nullptr), _scoreData({}) {
	_scoreData.energy = 1000;   //��ʼ������ֵΪ1000
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


//��������
bool MemoryCardScene::initWithScoreStrategy(std::shared_ptr<ScoreStrategy> scoreStrategy) {
	if (Layer::init() == false)
		return false;

	_scoreStrategy = scoreStrategy;
	
	initLevelDataList();  //��ʼ���ؿ�����
	//��������
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

	//�����������ͷ���
	_energyBar = EnergyBar::create();
	_energyBar->setPosition(visibleSize.width / 2, visibleSize.height - 100.0f);
	this->addChild(_energyBar);

	_scoreText = ScoreText::create();
	_scoreText->setPosition(Vec2(visibleSize.width - 20, visibleSize.height - 33));
	_scoreText->setScale(0.75f);
	this->addChild(_scoreText);

	//������ͣ��ť
	_pauseButton = ui::Button::create("pause.png");
	_pauseButton->setAnchorPoint(Vec2(0, 1));
	_pauseButton->setPosition(Vec2(0, visibleSize.height));
	addChild(_pauseButton);
	//����ť��ӵ�����¼�����
	_pauseButton->addClickEventListener([this](Ref* ref) {
		//����Ϸ������ͣ״̬��update������ִֹͣ��
		this->unscheduleUpdate();
		//Ȼ�󴴽��Ի���
		auto pauseBox = Pause::create();
		pauseBox->registerCallback([this, pauseBox]() {
			//�����ҵ��������ť�����Ƴ��Ի���
			pauseBox->removeFromParent();
			this->scheduleUpdate();
		}, []() {
			SceneMediator::getInstance()->gotoStartScene();
		});
		this->addChild(pauseBox);
	});
}

void MemoryCardScene::newGame() {
	//��ǰ�ؿ��ĸ�ֵ
	_nowLevel = -1;
	//������һ��
	nextLevel();
}

void MemoryCardScene::update(float t) {
	Layer::update(t);
	//��ÿ֡����һ��������ֵ
	_scoreData.energy -= this->_nowLeveldata.loss * t;
	//�����ǰ��������1000����ô����������1000
	if (_scoreData.energy > 1000) {
		_scoreData.energy = 1000;
	}
	else if (_scoreData.energy < 0) {
		_scoreData.energy = 0;
	}
	_energyBar->updateView(_scoreData.energy);
	_scoreText->updateView(_scoreData.score);
	//�����ǰ����ֵС�ڵ���0�������ǰ��Ϸ����
	if (_scoreData.energy <= 0) {
		//��Ϸ��������¼��ǰ����
		UserDefault::getInstance()->setIntegerForKey(NEW_SCORE, _scoreData.score);
		//���뵽���а���
		std::vector<int> scoreList; //���з���
		scoreList.push_back(_scoreData.score);	//��ǰ���·������뵽������
		for (int i = 0; i < 5; ++i) {
			int score = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("%s%d", RANK_SCORE, i).c_str(), 0);
			scoreList.push_back(score);
		}
		//���򣬴Ӵ�С
		std::sort(scoreList.begin(), scoreList.end(), [](int &a, int &b) { return a > b; });
		//��������������������а�������
		int rank = 0;
		for (auto i = scoreList.begin(); i < scoreList.end(); ++i) {
			UserDefault::getInstance()->setIntegerForKey(StringUtils::format("%s%d", RANK_SCORE, rank).c_str(), *i);
			rank++;
		}
		this->unscheduleUpdate();
		SceneMediator::getInstance()->gotoChartsScene();
	}
	if (_scoreData.energy < 600) {
		//���С��600��������������������ֵ�ĵݼ������ӣ� ���ݱ������м���
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume((600.0f - _scoreData.energy) / 600.0f);
	}
	else {
		//��������������������Ϊ0
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
	}
}

void MemoryCardScene::initLevelDataList() {
	//��csv�ļ����ص��ڴ���
	auto str = FileUtils::getInstance()->getStringFromFile("myleveldata.csv");
	//�������鱣��ÿһ������
	std::vector<std::string> rowList;
	//ʹ��c�������ַ������н��������������и���ַ����� �и����
	char *row = strtok((char*)str.c_str(), "\n");
	//whileѭ�������и�и����rowΪ��
	while (row) {
		//�и�ɹ���������
		rowList.push_back(row);
		//���Ҽ����и�, �ڶ����и��Ҫ�����ַ���
		row = strtok(nullptr, "\n");  
	}
	//���������н����е��и�
	for (auto row = rowList.begin(); row != rowList.end(); ++row) {
		//ֱ���и�3�Σ��ֱ�ȡ��
		//��ǰ������
		char *rows = strtok((char*)row->c_str(), ",");
		//��ǰ������
		char *columns = strtok(nullptr, ",");
		//�������ֵ
		char *loss = strtok(nullptr, ",");
		//��ȡ�������ݽ����ж�,���������һ��Ϊ��������
		if (rows == nullptr || columns == nullptr || loss == nullptr) {
			continue;
		}
		//����LevelData���󣬽��и�ֵ
		LevelData level;
		level.rows = atoi(rows);
		level.columns = atoi(columns);
		level.loss = atoi(loss);
		//�ж�����*��������С�ڵ���0����*�б��뱻2���������ֵ�������Ǹ���������ؿ�Ҳû����
		if (level.rows * level.columns <= 0 || (level.columns * level.rows) % 2 != 0 || level.loss < 0) {
			continue;
		}
		//��������Ĺؿ����ݼ��뵽������
		_levelDatalist.push_back(level);
	}
	_allLevel = static_cast<int>(_levelDatalist.size());  //��ֵ�ܹؿ�������
}

void MemoryCardScene::nextLevel() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//ÿ�����µ�һ��_nowLevel+1
	_nowLevel++;
	if (_nowLevel >= _allLevel) {
		_nowLevel = 0;
	}
	//ֱ�Ӵ��Ѿ�������Ĺؿ�������ȡ������
	_nowLeveldata = _levelDatalist[_nowLevel];

	//ʵ����һ���ؿ�
	auto level = _nowLevelLayer = MemoryCardLevel::create(_nowLeveldata);
	auto levelSize = level->getContentSize();
	level->setAnchorPoint(Vec2(0.5, 0.5));
	level->ignoreAnchorPointForPosition(false);
	level->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	//����һ���ű���
	auto scale = visibleSize.width / (levelSize.width + 100);
	//���ؿ������ƶ�һ��������
	level->setPositionY(level->getPositionY() - 80 * scale);

	level->setScale(2);
	level->setOpacity(0);
	this->addChild(level);
	//���һ����Ч�������¹ؿ�ʱ
	level->runAction(Spawn::create(FadeIn::create(0.25), ScaleTo::create(0.25, scale), NULL));
	_nowLevelLayer = level;  //��ֵ��ǰ�ؿ�

	level->registerCallfunc([this](CardData* cardA, CardData* cardB) {
		_scoreStrategy->execute(&_scoreData, cardA, cardB);
		log("score:%d, energy:%d, macCountinuous:%d", _scoreData.score, _scoreData.energy, _scoreData.maxContinuous);
	}, [this]() {
		log("complete\n");
		//��ÿ�ؽ���ʱ�������Ƴ�
		_nowLevelLayer->removeFromParent();
		nextLevel();
	});

}