#include "ChartsScene.h"
#include "StartScene.h"
#include "UserData.h"
#include "SceneMediator.h"
ChartsScene::ChartsScene():
	_background(nullptr),
	_backBut(nullptr),
	_newScoreTitle(nullptr),
	_newScore(nullptr),
	_chartScoreTitle(nullptr){

}

ChartsScene::~ChartsScene() {

}

Scene* ChartsScene::createScene() {
	auto layer = ChartsScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
}

bool ChartsScene::init() {
	if (Layer::init() == false) {
		return false;
	}
	auto viewSize = Director::getInstance()->getVisibleSize();

	_background = BackGround::create();
	this->addChild(_background);
	
	_backBut = ui::Button::create("back.png");
	_backBut->setAnchorPoint(Vec2(0, 1));  //由于要将其放在屏幕的左上角
	_backBut->setPosition(Vec2(10, viewSize.height - 10));
	this->addChild(_backBut);

	_backBut->addClickEventListener([this](Ref* ref) {
		SceneMediator::getInstance()->gotoStartScene();
	});

	_newScoreTitle = Sprite::create("new_score.png");
	_newScoreTitle->setAnchorPoint(Vec2(0, 0.5f));
	_newScoreTitle->setPosition(Vec2(10, viewSize.height / 2 + 235));
	this->addChild(_newScoreTitle);

	_newScore = ui::TextAtlas::create("0", "number.png", 63, 83, "0");
	_newScore->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 + 135));
	//获取当前积分,根据key获取最新积分，如果积分不能再则默认返回0
	auto newScore = UserDefault::getInstance()->getIntegerForKey(NEW_SCORE, 0); //定义了一个所有会用到的Key头文件
	//使用format进行数字对字符串的转换
	_newScore->setString(StringUtils::format("%d", newScore));
	this->addChild(_newScore);

	_chartScoreTitle = Sprite::create("charts_score.png");
	_chartScoreTitle->setAnchorPoint(Vec2(0, 0.5f));
	_chartScoreTitle->setPosition(Vec2(10, viewSize.height / 2 + 35));
	this->addChild(_chartScoreTitle);

	//创建排行榜
	for (int i = 0; i < 5; ++i) {  //5个排名
		//当前得分,由于要记录5个名词，因此组合一下字符串，使用索引来指定不同的排名
		auto score = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("%s%d", RANK_SCORE,
			i).c_str(), 0);
		//创建一个行
		auto row = createChart(i, score);
		//设置位置，根据索引顺序的向下排列显示名次
		row->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 + 95 - 100 * i));
		this->addChild(row);
	}

	return true;
}

Node* ChartsScene::createChart(int rank, int score) {  //1.当前排名， 2.当前分数
	auto viewSize = Director::getInstance()->getVisibleSize();
	auto row = Node::create();
	//创建单行-名次
	auto r = ui::TextAtlas::create(StringUtils::format("%d", rank + 1), "number.png", 63, 83, "0");
	//创建当前-积分
	auto s = ui::TextAtlas::create(StringUtils::format("%d", score), "number.png", 63, 83, "0");

	//名次瞄点左对齐，分数瞄点右对齐
	r->setAnchorPoint(Vec2(0, 0.5));
	s->setAnchorPoint(Vec2(1, 0.5));
	r->setPosition(Vec2(30, 0));
	s->setPosition(Vec2(viewSize.width - 30, 0));
	//添加至行
	row->addChild(r);
	row->addChild(s);

	row->setContentSize(Size(viewSize.width, 100));
	row->setAnchorPoint(Vec2(0.5, 0.5));

	return row;
}
