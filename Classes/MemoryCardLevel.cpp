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
	//���ʧ��ɾ�����󣬷��ؿ�
	CC_SAFE_DELETE(cl);
	return nullptr;
}

bool MemoryCardLevel::initWithLevelData(LevelData levelData) {
	if (Layer::init() == false) { //���ȵ��ø���ĳ�ʼ����������ʼ��ͼ�㡣���ʧ���򷵻�false
		return false;
	}
	//�ж������Ƿ���ϱ�׼
	if (levelData.rows * levelData.columns % 2 != 0)
		return false;
	_levelData = levelData;
	//��ʼ����Ƭ����
	initCardLayout();
	initTouchEvent();
	return true;
}

void MemoryCardLevel::initCardLayout() {
	//������ɫ�������
	int backID = CCRANDOM_0_1() * 8;  //8��
	//��ʼ����Ƭ����
	for (auto row = 0; row < _levelData.rows; ++row) {
		std::vector<Card*> r(_levelData.columns);
		_cardTable.push_back(r);
	}
	//����һ��˫��ѭ��������ʼ����Ƭ
	CardFactory factory;
	int number = 0 ; //��ż�¼
	//������п�Ƭ
	Vector<Card*> cardList;  //�洢���еĿ�Ƭ
	for (number = 0; number < _levelData.columns * _levelData.rows; ++number) {
		//������Ƭ
		Card* card = dynamic_cast<Card*>(factory.createCard(backID, number / 2));
		cardList.pushBack(card);
	}

	//����ʣ�࿨Ƭ�������
	this->_unfinishedCard = number;

	//ÿ���ſ�Ƭ������һ��˫��ѭ����
	for(int row = 0; row < _levelData.rows; ++row)
		for (int column = 0; column < _levelData.columns; ++column) {
			//�����ȡ��Ƭ
			int size = cardList.size();
			//int at = CCRANDOM_0_1() *cardList.size();
			auto card = cardList.at(CCRANDOM_0_1() * size);
			//ȡ������Ҫ�Ƴ�ѡ��Ŀ�Ƭ
			cardList.eraseObject(card);
			//���õ�ǰ�У���ǰ��

			//���ÿ�Ƭ������
			int space = 20;  //������룬��Ƭ���������Ҽ��
			//��ȡһ����Ƭ�Ĵ�С
			//card->setScale(0.5);
			Size cardSize = card->getContentSize();
			//log("SizeWidth: %d, SizeHeight: %d", cardSize.width, cardSize.height);
			
			//��Ƭλ�ã����㷽������Ƭ���+���*��ǰ��(��Ӱ�����y���λ��)+��Ƭ���/2(��Ϊ��Ƭê�����м䣬��λ���Ǵ����½����еģ�
			card->setPosition(Vec2 ((cardSize.width + space) * column + cardSize.width / 2,
				(cardSize.height + space) * row + cardSize.height / 2));
			_cardTable[row][column] = card;

			card->getCardData()->row = row;
			card->getCardData()->column = column;
			//�������ʾ�б�
			this->addChild(card);
			//����ͼ���С,�������һ���λ�ý��м���
			if (column == _levelData.columns - 1 && row == _levelData.rows - 1) {
				//����ͼ���С��������Ƭ���+����ռ�*��ǰ����+��Ƭ���
				this->setContentSize(Size((cardSize.width + space) * column + cardSize.width,
										  (cardSize.height + space) * row + cardSize.height * 1 ));
			}
		}
}
//ʵ��ע��ص�����
void MemoryCardLevel::registerCallfunc(std::function<void(CardData* cardA, CardData* cardB)> pairCallBack, std::function<void()> completeCallBack) {
	_pairCallback = pairCallBack;
	_completeCallBack = completeCallBack;
}
//��ʼ�������¼��ĺ���
void MemoryCardLevel::initTouchEvent() {
	//1.��鵱ǰ�������Ƿ�������ؿ��������ڣ�����������ڶ����֣���鵱ǰ���е������ſ�Ƭ
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *event) {
		//����һ���㣬ת����������ǰ�ռ������
		Point locationInNode = this->convertToNodeSpace(touch->getLocation());
		//��ȡ��ǰ���ݴ��С
		Size s = this->getContentSize();
		//����һ������
		Rect rect = Rect(0, 0, s.width, s.height);
		//�жϵ�ǰ�����λ���Ƿ��ڹؿ�������
		if (rect.containsPoint(locationInNode))
			return true;
		return false;
	};

	listener->onTouchEnded = [&](Touch *touch, Event *event) {
		//�����ҵ���ǰѡ�еĿ�Ƭ����Ҫ�ж��ǵ�һ��ѡ�л��ǵڶ��Σ�����ǵ�һ���򽫿�Ƭ������������ǵڶ�����Ҫ
		//��ǰһ�ſ�Ƭ���жԱȣ���ƥ�������������򽫿�Ƭ��ԭ
		Point locationInNode = this->convertToNodeSpace(touch->getLocation());
		//���ҳ���ǰ�����Ŀ�Ƭ
		Card *selCard = nullptr;
		//��ⴥ�����Ƿ�͵�ǰ���������غ�
		for (auto row = 0; row < _levelData.rows; ++row) {
			auto cards = _cardTable[row];  //Ϊ�����Ч�ʣ���ÿһ�ж���ȡ����
			for (auto column = 0; column < _levelData.columns; ++column) {
				auto card = cards[column];  //��ȡ������Ƭ
				//�����ƬΪ�գ�����������ô����
				if (card == nullptr)
					continue;
				auto size = card->getContentSize();
				auto pos = card->getPosition();
				auto rect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);
				//�ж�������Ƿ������������
				if (rect.containsPoint(locationInNode)) {
					//����ڣ���¼��Ƭ������
					selCard = card;
					break;
				}
			}
		}//for
		//�����ǰ�����Ŀ�ƬΪ�ջ�����֮ǰ�����Ŀ�Ƭһ����ҲҪ����
		if (selCard == nullptr || _selCardA == selCard) {
			return;
		}
		//����ǵ�һ��ѡ�����ſ�Ƭ,
		if (_selCardA == nullptr) {
			//��ôֱ�Ӹ�ֵ���ұ�������
			_selCardA = selCard;
			//�����Ŷ���,������ת������
			_selCardA->flipToFront();
		}
		else {//����ǵڶ��δ�����ͼƬ��
			_selCardB = selCard;
			//Ҳ�������������һ����ƥ��ɹ���һ����ƥ��ʧ��
			//��ȡ������Ƭ������
			auto dataA = _selCardA->getCardData();
			auto dataB = _selCardB->getCardData();

			//֪ͨ��������п�Ƭ��Ҫƥ�䣬֪ͨ�ⲿ�ص�
			if (_pairCallback != nullptr) { //���û��ע�ᣬ����Ҫ֪ͨ
				this->_pairCallback(dataA, dataB);
			}
	
			//ˢ����ͼ
			//�ж����ſ�Ƭ��number�Ƿ�ƥ��
			if (dataA->number == dataB->number) {
				//��Գɹ���������---�����������������ǵڶ��η�ת�Ŀ�Ƭ������ʱ������
				//��ˣ��Ƚ��ڶ��η�ת�Ŀ�Ƭ��ת������
				auto cardA = _selCardA;
				auto cardB = _selCardB;
				_selCardB->flipToFront([&, cardA, cardB]() { //��ת���������
					//���붯��
					cardA->runAction(Sequence::create(Spawn::create(FadeOut::create(0.25), ScaleTo::create(0.25, 0.25), NULL),
									CallFunc::create([this, cardA]() {
						//����������ʽΪ��ը������Ч��
						auto p = ParticleExplosion::create();
						p->setPosition(cardA->getPosition());

						p->setStartColor(Color4F(1, 1, 1, 0.25));
						p->setEndColor(Color4F(1, 1, 1, 0.25));
						//���ÿɱ���ɫ
						p->setStartColorVar(Color4F(0.1, 0.1, 0.1, 0.25));
						p->setEndColorVar(Color4F(0.1, 0.1, 0.1, 0.25));
						//���������ٶȼ��ɱ䷶Χ
						p->setSpeed(1200);
						p->setSpeedVar(600);
						//����������Ϊ�Զ��ͷţ���������Ч��������������Զ��ͷ�
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
						//���ÿɱ���ɫ
						p->setStartColorVar(Color4F(0.1, 0.1, 0.1, 0.25));
						p->setEndColorVar(Color4F(0.1, 0.1, 0.1, 0.25));
						//���������ٶȼ��ɱ䷶Χ
						p->setSpeed(1200);
						p->setSpeedVar(600);
						//����������Ϊ�Զ��ͷţ���������Ч��������������Զ��ͷ�
						p->setAutoRemoveOnFinish(true);

						this->addChild(p);
						cardB->removeFromParent();
					}), NULL));
				});

				//ɾ��֮����Ҫ�ı�����
				//1.����Ӧ�ĸ�����Ϊ��
				_cardTable[dataA->row][dataA->column] = nullptr;
				_cardTable[dataB->row][dataB->column] = nullptr;
				//2.����ס����ѡ��Ŀ�ƬҲ��Ϊ��
				_selCardA = nullptr;
				_selCardB = nullptr;
				//3.ʣ�࿨Ƭ��-2
				this->_unfinishedCard -= 2;
				//ȫ����ɣ������ʣ�࿨Ƭ��Ϊ0�����Ѿ�ע��������ص�����
				if (_unfinishedCard == 0 && _completeCallBack != nullptr) {
					_completeCallBack();
				}
			}
			else {//���ʧ�ܣ��򷭻�
				auto cardA = _selCardA;
				auto cardB = _selCardB;
				//���Ƚ��ڶ��εĿ�Ƭ��������, Ȼ���ٽ���һ�εĿ�Ƭ����һ�𷵻ص�����
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