#include "EngTestScene.h"

USING_NS_CC;

static const int kGridWidth = 10;
static const int kGridHeight = 5;

EngTestScene::EngTestScene()
:
    mNodePositions(),
    mTableLocations(),
    mCharacterPosition(135,135),
	mCurrentClickable(true)
{
    mTableLocations.push_back(GridCoordinate(2,1));
    mTableLocations.push_back(GridCoordinate(2,2));
    mTableLocations.push_back(GridCoordinate(2,3));
    mTableLocations.push_back(GridCoordinate(3,3));
    mTableLocations.push_back(GridCoordinate(4,3));
    mTableLocations.push_back(GridCoordinate(5,3));
    mTableLocations.push_back(GridCoordinate(6,3));
    mTableLocations.push_back(GridCoordinate(7,3));
    mTableLocations.push_back(GridCoordinate(7,2));
    mTableLocations.push_back(GridCoordinate(7,1));
    mTableLocations.push_back(GridCoordinate(7,0));
}

Scene* EngTestScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = EngTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool EngTestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(EngTestScene::menuCloseCallback, this));

	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// The shuffle tables button
	auto randomTblItem = MenuItemImage::create(
		"button_shuffle.png",
		"button_shuffle.png",
		CC_CALLBACK_1(EngTestScene::randomizeTablesCallback, this));

	randomTblItem->setPosition(Point(origin.x + randomTblItem->getContentSize().width / 2,
		origin.y + visibleSize.height - randomTblItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, randomTblItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "EngTestScene" splash screen"
    auto sprite = Sprite::create("bg.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
	auto nodeSpriteOrig = Sprite::create("node.png");
	mGridPixelWidth = nodeSpriteOrig->getBoundingBox().size.width;
	mGridPixelHeight = nodeSpriteOrig->getBoundingBox().size.height;

    // Add a node grid
    for (int x = 0 ; x < kGridWidth ; ++x)
    {
        mNodePositions.push_back(std::vector<NodePosition>());
        for (int y = 0 ; y < kGridHeight ; ++y)
        {
            auto nodeSprite = Sprite::create("node.png");
            mNodePositions[x].push_back(NodePosition(nodeSprite->getBoundingBox().size.width *x, nodeSprite->getBoundingBox().size.height*y));
            nodeSprite->setPosition(mNodePositions[x][y]);
            nodeSprite->setAnchorPoint(cocos2d::Point(0,0));
            this->addChild(nodeSprite);
        }
    }
    
    // Add some tables
    for (auto it : mTableLocations)
    {
        auto tableSprite = Sprite::create("table.png");
        tableSprite->setPosition(mNodePositions[it.x][it.y]);
		tableSprite->setAnchorPoint(cocos2d::Point(0, 0));
		mTableSprites.push_back(tableSprite);
		mOccupiedGrid.insert(it);
        this->addChild(tableSprite);
    }
    
    // Add the character
    mCharacterSprite = Sprite::create("flo.png");
	mCharacterSprite->setPosition(mCharacterPosition);
	mCharacterSprite->setAnchorPoint(cocos2d::Point(0.5, 0.2));
	this->addChild(mCharacterSprite);

	auto listener = EventListenerTouchOneByOne::create();

	// Adding the touch events
	listener->onTouchBegan = CC_CALLBACK_2(EngTestScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(EngTestScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(EngTestScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void EngTestScene::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void EngTestScene::randomizeTablesCallback(Object* pSender)
{
	if (!mCurrentClickable) {
		return;
	}
	moveCharaterTo(Point(0, 0), CallFunc::create([&]() { scrambleTables();  mCurrentClickable = true; }));
}

bool EngTestScene::onTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
//	cout << "onTouchBegan\n";
	if (!mCurrentClickable) {
		return false;
	}
	auto pos = touch->getLocation();
	Point index((int)(pos.x / mGridPixelWidth), (int)(pos.y / mGridPixelHeight));
	if (index.x < 0 || index.x >= kGridWidth || index.y < 0 || index.y >= kGridHeight) {
		return false;
	}
	if (mOccupiedGrid.count(index)) {
		cout << "Occupied.\n";
	} else {
		moveCharaterTo(index, CallFunc::create([&]() { mCurrentClickable = true; }));
	}
	return true;
}

void EngTestScene::onTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
//	cout << "onTouchMoved\n";
}

void EngTestScene::onTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
//	cout << "onTouchEnded\n";
}

void EngTestScene::scrambleTables()
{
	mCurrentClickable = false;
	Point playerPosition = 
		Point((int)(mCharacterSprite->getPosition().x / mGridPixelWidth), 
		      (int)(mCharacterSprite->getPosition().y / mGridPixelHeight));
	int currentX = 1;
	int currentY = rand() % kGridHeight;
	int index = 0;
	bool isHori = true;
	mOccupiedGrid.clear();
	while (index < mTableLocations.size()) {
		int partSize;
		int stepX, stepY;
		if (isHori) {
			partSize = rand() % 2 + 1;
			stepX = 1;
			stepY = 0;
		} else {
			int top = kGridHeight - currentY - 1;
			int bottom = currentY;
			if (top > bottom) {
				stepY = 1;
				partSize = rand() % (top - 1) + 1;
			} else {
				stepY = -1;
				partSize = rand() % (bottom - 1) + 1;
			}
			stepX = 0;
		}
		for (int i = 0; i < partSize && index < mTableLocations.size(); i++, index++, currentX += stepX, currentY += stepY) {
			if (currentX == playerPosition.x && currentY == playerPosition.y) {
				index--;
				continue;
			}
			mTableLocations[index] = Point(currentX, currentY);
			mOccupiedGrid.insert(mTableLocations[index]);
		}
		isHori = !isHori;
	}

	for (int i = 0; i < mTableSprites.size() - 1; i++) {
		Point tar(mTableLocations[i].x * mGridPixelWidth, mTableLocations[i].y * mGridPixelHeight);
		mTableSprites[i]->runAction(MoveTo::create(0.5f, tar));
	}
	Point tar(mTableLocations.back().x * mGridPixelWidth, mTableLocations.back().y * mGridPixelHeight);
	mTableSprites.back()->runAction(
		Sequence::createWithTwoActions(MoveTo::create(0.5f, tar), CallFunc::create([&]() { mCurrentClickable = true; })));
}

bool EngTestScene::moveCharaterTo(Point targetIndex, CallFunc *finishedOps)
{
	Point playerPosition =
		Point((int)(mCharacterSprite->getPosition().x / mGridPixelWidth),
		      (int)(mCharacterSprite->getPosition().y / mGridPixelHeight));
	targetIndex.x = (int)targetIndex.x;
	targetIndex.y = (int)targetIndex.y;
	if (playerPosition == targetIndex) {
		return false;
	}
	mCurrentClickable = false;
	auto actions = breadthFirstSearch(
		playerPosition, targetIndex, 
		Point(kGridWidth, kGridHeight), Point(mGridPixelWidth, mGridPixelHeight), 
		mOccupiedGrid, 0.1f);
	mCharacterSprite->runAction(
		Sequence::create(Sequence::create(actions), finishedOps, nullptr));

	return true;
}
