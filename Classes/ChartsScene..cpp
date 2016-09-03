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
	_backBut->setAnchorPoint(Vec2(0, 1));  //����Ҫ���������Ļ�����Ͻ�
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
	//��ȡ��ǰ����,����key��ȡ���»��֣�������ֲ�������Ĭ�Ϸ���0
	auto newScore = UserDefault::getInstance()->getIntegerForKey(NEW_SCORE, 0); //������һ�����л��õ���Keyͷ�ļ�
	//ʹ��format�������ֶ��ַ�����ת��
	_newScore->setString(StringUtils::format("%d", newScore));
	this->addChild(_newScore);

	_chartScoreTitle = Sprite::create("charts_score.png");
	_chartScoreTitle->setAnchorPoint(Vec2(0, 0.5f));
	_chartScoreTitle->setPosition(Vec2(10, viewSize.height / 2 + 35));
	this->addChild(_chartScoreTitle);

	//�������а�
	for (int i = 0; i < 5; ++i) {  //5������
		//��ǰ�÷�,����Ҫ��¼5�����ʣ�������һ���ַ�����ʹ��������ָ����ͬ������
		auto score = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("%s%d", RANK_SCORE,
			i).c_str(), 0);
		//����һ����
		auto row = createChart(i, score);
		//����λ�ã���������˳�������������ʾ����
		row->setPosition(Vec2(viewSize.width / 2, viewSize.height / 2 + 95 - 100 * i));
		this->addChild(row);
	}

	return true;
}

Node* ChartsScene::createChart(int rank, int score) {  //1.��ǰ������ 2.��ǰ����
	auto viewSize = Director::getInstance()->getVisibleSize();
	auto row = Node::create();
	//��������-����
	auto r = ui::TextAtlas::create(StringUtils::format("%d", rank + 1), "number.png", 63, 83, "0");
	//������ǰ-����
	auto s = ui::TextAtlas::create(StringUtils::format("%d", score), "number.png", 63, 83, "0");

	//�����������룬��������Ҷ���
	r->setAnchorPoint(Vec2(0, 0.5));
	s->setAnchorPoint(Vec2(1, 0.5));
	r->setPosition(Vec2(30, 0));
	s->setPosition(Vec2(viewSize.width - 30, 0));
	//�������
	row->addChild(r);
	row->addChild(s);

	row->setContentSize(Size(viewSize.width, 100));
	row->setAnchorPoint(Vec2(0.5, 0.5));

	return row;
}
