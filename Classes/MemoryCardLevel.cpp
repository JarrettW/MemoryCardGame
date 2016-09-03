#include "MemoryCardLevel.h"
#include "CardFactory.h"
#include "SimpleAudioEngine.h"

MemoryCardLevel::MemoryCardLevel():
	_selCardA(nullptr),
	_selCardB(nullptr),
	_unfinishedCard(0){
	this->setCascadeOpacityEnabled(true);
}

MemoryCardLevel::~MemoryCardLevel() {
	this->removeAllChildren();
	//this->_eventDispatcher->removeAllEventListeners();
}

MemoryCardLevel* MemoryCardLevel::create(LevelData levelData) {
	MemoryCardLevel *cl = new MemoryCardLevel();
	if (cl && cl->initWithLevelData(levelData)) {
		cl->autorelease();
		return cl;
	}
	//如果失败删除对象，返回空
	CC_SAFE_DELETE(cl);
	return nullptr;
}

bool MemoryCardLevel::initWithLevelData(LevelData levelData) {
	if (Layer::init() == false) { //首先调用父类的初始化函数，初始化图层。如果失败则返回false
		return false;
	}
	//判断行列是否符合标准
	if (levelData.rows * levelData.columns % 2 != 0)
		return false;
	_levelData = levelData;
	//初始化卡片布局
	initCardLayout();
	initTouchEvent();
	return true;
}

void MemoryCardLevel::initCardLayout() {
	//产生颜色的随机数
	int backID = CCRANDOM_0_1() * 8;  //8张
	//初始化卡片数组
	for (auto row = 0; row < _levelData.rows; ++row) {
		std::vector<Card*> r(_levelData.columns);
		_cardTable.push_back(r);
	}
	//创建一个双重循环体来初始化卡片
	CardFactory factory;
	int number = 0 ; //编号记录
	//随机排列卡片
	Vector<Card*> cardList;  //存储所有的卡片
	for (number = 0; number < _levelData.columns * _levelData.rows; ++number) {
		//创建卡片
		Card* card = dynamic_cast<Card*>(factory.createCard(backID, number / 2));
		cardList.pushBack(card);
	}

	//设置剩余卡片，即编号
	this->_unfinishedCard = number;

	//每两张卡片都会有一个双重循环体
	for(int row = 0; row < _levelData.rows; ++row)
		for (int column = 0; column < _levelData.columns; ++column) {
			//随机获取卡片
			int size = cardList.size();
			//int at = CCRANDOM_0_1() *cardList.size();
			auto card = cardList.at(CCRANDOM_0_1() * size);
			//取出来后，要移除选择的卡片
			cardList.eraseObject(card);
			//设置当前行，当前列

			//设置卡片的坐标
			int space = 20;  //间隔距离，卡片的上下左右间距
			//获取一个卡片的大小
			//card->setScale(0.5);
			Size cardSize = card->getContentSize();
			//log("SizeWidth: %d, SizeHeight: %d", cardSize.width, cardSize.height);
			
			//卡片位置，计算方法：卡片宽度+间距*当前列(列影响的是y轴的位置)+卡片宽度/2(因为卡片锚点在中间，而位置是从左下角排列的）
			card->setPosition(Vec2 ((cardSize.width + space) * column + cardSize.width / 2,
				(cardSize.height + space) * row + cardSize.height / 2));
			_cardTable[row][column] = card;

			card->getCardData()->row = row;
			card->getCardData()->column = column;
			//添加至显示列表
			this->addChild(card);
			//设置图层大小,仅在最后一格的位置进行计算
			if (column == _levelData.columns - 1 && row == _levelData.rows - 1) {
				//设置图层大小方法：卡片宽度+间隔空间*当前的列+卡片宽度
				this->setContentSize(Size((cardSize.width + space) * column + cardSize.width,
										  (cardSize.height + space) * row + cardSize.height * 1 ));
			}
		}
}
//实现注册回调函数
void MemoryCardLevel::registerCallfunc(std::function<void(CardData* cardA, CardData* cardB)> pairCallBack, std::function<void()> completeCallBack) {
	_pairCallback = pairCallBack;
	_completeCallBack = completeCallBack;
}
//初始化触摸事件的函数
void MemoryCardLevel::initTouchEvent() {
	//1.检查当前触摸点是否在这个关卡的区域内，如果是则进入第二部分，检查当前点中的是哪张卡片
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *event) {
		//创建一个点，转换到触摸当前空间的坐标
		Point locationInNode = this->convertToNodeSpace(touch->getLocation());
		//获取当前内容打大小
		Size s = this->getContentSize();
		//创建一个矩形
		Rect rect = Rect(0, 0, s.width, s.height);
		//判断当前点击的位置是否在关卡区域内
		if (rect.containsPoint(locationInNode))
			return true;
		return false;
	};

	listener->onTouchEnded = [&](Touch *touch, Event *event) {
		//我们找到当前选中的卡片，还要判断是第一次选中还是第二次，如果是第一次则将卡片翻过来，如果是第二次则要
		//与前一张卡片进行对比，若匹配则消除，否则将卡片还原
		Point locationInNode = this->convertToNodeSpace(touch->getLocation());
		//查找出当前触摸的卡片
		Card *selCard = nullptr;
		//检测触摸点是否和当前触摸区域重合
		for (auto row = 0; row < _levelData.rows; ++row) {
			auto cards = _cardTable[row];  //为了提高效率，把每一行都获取出来
			for (auto column = 0; column < _levelData.columns; ++column) {
				auto card = cards[column];  //获取单个卡片
				//如果卡片为空，即被消除那么跳过
				if (card == nullptr)
					continue;
				auto size = card->getContentSize();
				auto pos = card->getPosition();
				auto rect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
				//判断这个点是否在这个区域内
				if (rect.containsPoint(locationInNode)) {
					//如果在，记录卡片并跳出
					selCard = card;
					break;
				}
			}
		}//for
		//如果当前触摸的卡片为空或者与之前触摸的卡片一样，也要跳出
		if (selCard == nullptr || _selCardA == selCard) {
			return;
		}
		//如果是第一次选择这张卡片,
		if (_selCardA == nullptr) {
			//那么直接赋值并且保存下来
			_selCardA = selCard;
			//并播放动画,即让它转到正面
			_selCardA->flipToFront();
		}
		else {//如果是第二次打开这张图片，
			_selCardB = selCard;
			//也会有两种情况，一种是匹配成功，一种是匹配失败
			//获取两个卡片的数据
			auto dataA = _selCardA->getCardData();
			auto dataB = _selCardB->getCardData();

			//通知外层现在有卡片需要匹配，通知外部回调
			if (_pairCallback != nullptr) { //如果没有注册，则不需要通知
				this->_pairCallback(dataA, dataB);
			}
	
			//刷新视图
			//判断两张卡片的number是否匹配
			if (dataA->number == dataB->number) {
				//配对成功，则消除---不是马上消除，而是第二次翻转的卡片到正面时再消除
				//因此，先将第二次翻转的卡片翻转到正面
				auto cardA = _selCardA;
				auto cardB = _selCardB;
				_selCardB->flipToFront([&, cardA, cardB]() { //翻转后接着消除
					//加入动画
					cardA->runAction(Sequence::create(Spawn::create(FadeOut::create(0.25), ScaleTo::create(0.25, 0.25), NULL),
									CallFunc::create([this, cardA]() {
						//动画表现形式为爆炸的粒子效果
						auto p = ParticleExplosion::create();
						p->setPosition(cardA->getPosition());

						p->setStartColor(Color4F(1, 1, 1, 0.25));
						p->setEndColor(Color4F(1, 1, 1, 0.25));
						//设置可变颜色
						p->setStartColorVar(Color4F(0.1, 0.1, 0.1, 0.25));
						p->setEndColorVar(Color4F(0.1, 0.1, 0.1, 0.25));
						//设置粒子速度及可变范围
						p->setSpeed(1200);
						p->setSpeedVar(600);
						//将粒子设置为自动释放，即当粒子效果播放完后，它将自动释放
						p->setAutoRemoveOnFinish(true);

						this->addChild(p);
						cardA->removeFromParent();

						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("ye.mp3");

					}), NULL));
					cardB->runAction(Sequence::create(Spawn::create(FadeOut::create(0.25), ScaleTo::create(0.25, 0.25), NULL),
									CallFunc::create([this, cardB]() {
						auto p = ParticleExplosion::create();
						p->setPosition(cardB->getPosition());
						p->setStartColor(Color4F(1, 1, 1, 0.25));
						p->setEndColor(Color4F(1, 1, 1, 0.25));
						//设置可变颜色
						p->setStartColorVar(Color4F(0.1, 0.1, 0.1, 0.25));
						p->setEndColorVar(Color4F(0.1, 0.1, 0.1, 0.25));
						//设置粒子速度及可变范围
						p->setSpeed(1200);
						p->setSpeedVar(600);
						//将粒子设置为自动释放，即当粒子效果播放完后，它将自动释放
						p->setAutoRemoveOnFinish(true);

						this->addChild(p);
						cardB->removeFromParent();
					}), NULL));
				});

				//删除之后需要改变数据
				//1.将相应的格子设为空
				_cardTable[dataA->row][dataA->column] = nullptr;
				_cardTable[dataB->row][dataB->column] = nullptr;
				//2.将记住两个选择的卡片也设为空
				_selCardA = nullptr;
				_selCardB = nullptr;
				//3.剩余卡片数-2
				this->_unfinishedCard -= 2;
				//全部完成，即如果剩余卡片数为0并且已经注册了这个回调函数
				if (_unfinishedCard == 0 && _completeCallBack != nullptr) {
					_completeCallBack();
				}
			}
			else {//配对失败，则翻回
				auto cardA = _selCardA;
				auto cardB = _selCardB;
				//需先将第二次的卡片翻到正面, 然后再将第一次的卡片和它一起返回到反面
				_selCardB->flipToFront([&, cardA, cardB]() {
					cardA->flipToBack();
					cardB->flipToBack();
				});
				_selCardA = nullptr;
				_selCardB = nullptr;
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}