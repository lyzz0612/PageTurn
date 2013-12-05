//
//  HelloPageTurn.cpp
//  PageTurn
//
//  Created by lingyun on 13-12-5.
//
//

#include "HelloPageTurn.h"

HelloPageTurn::HelloPageTurn()
{
    
}
HelloPageTurn::~HelloPageTurn()
{
    
}
CCScene* HelloPageTurn::scene()
{
    CCScene* scene = CCScene::create();
    if(scene)
    {
        HelloPageTurn* layer = HelloPageTurn::create();
        if(layer)
            scene->addChild(layer);
    }
    return scene;
}
bool HelloPageTurn::init()
{
    if(!CCLayer::init())
        return false;
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* back = CCSprite::create("back.png");
    back->setRotation(2.1f);
    back->setPosition(ccp(winsize.width / 2, winsize.height / 2));
    
    CCSprite* left = CCSprite::create("left.png");
    left->setPosition(ccp(winsize.width / 2 + 180, winsize.height / 2 - 20));
    PageTurnLayer* layer = PageTurnLayer::create(left, PAGEPOSRIGHT);
    layer->setAnchorPoint(CCPointZero);
    layer->setPosition(CCPointZero);
    layer->getTurn()->registerTurnedCallBack(this, callfunc_selector(HelloPageTurn::TurnedCallback));
    
    CCSprite* left1 = CCSprite::create("left.png");
    left1->setPosition(ccp(winsize.width / 2 - 180, winsize.height / 2 - 20));
    PageTurnLayer* layer1 = PageTurnLayer::create(left1, PAGEPOSLEFT);
    layer1->setAnchorPoint(CCPointZero);
    layer1->setPosition(CCPointZero);
    
    this->addChild(layer, 1, 11);
    this->addChild(layer1, 1, 12);
    this->addChild(back, 0, 10);
    return true;
}
void HelloPageTurn::TurnedCallback()
{
    this->removeChildByTag(11);
}


