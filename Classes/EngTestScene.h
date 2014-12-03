#ifndef __ENGTEST_SCENE_H__
#define __ENGTEST_SCENE_H__

#include "cocos2d.h"
#include "Utility.h"
// #include <base/CCGeometry.h>

class EngTestScene : public cocos2d::Layer
{
public:
    EngTestScene();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
	void randomizeTablesCallback(Object* pSender);

	virtual bool onTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	virtual void onTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	virtual void onTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(EngTestScene);

	void scrambleTables();
	bool moveCharaterTo(Point targetIndex, CallFunc *finishedOps);
    
private:
    typedef cocos2d::Point NodePosition;
    typedef cocos2d::Point GridCoordinate;
    
    // World position of all the level nodes
	std::vector<std::vector<NodePosition> > mNodePositions;
	std::unordered_set<NodePosition, cocos2d::PointHash, cocos2d::PointCmp> mOccupiedGrid;

	int mGridPixelWidth;
	int mGridPixelHeight;
    
    // Location of the tables (in grid positions
	std::vector<GridCoordinate> mTableLocations;
	std::vector<cocos2d::Sprite *> mTableSprites;
    
    // Location of the character in world space
	cocos2d::Point mCharacterPosition;
	cocos2d::Sprite *mCharacterSprite;

	bool mCurrentClickable;
};

#endif // __ENGTEST_SCENE_H__
