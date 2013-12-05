//
//  PageTurnLayer.h
//  PageTurn
//
//  Created by lingyun on 13-12-2.
//
//

#ifndef __PageTurn__PageTurnLayer__
#define __PageTurn__PageTurnLayer__

#include <iostream>
#include "cocos2d.h"
#include "CCActionPageTurn3D.h"
#include "PageTurn.h"
USING_NS_CC;

enum PagePos
{
    PAGEPOSLEFT = 0,
    PAGEPOSRIGHT,
};
class PageTurnLayer : public CCLayer
{
public:
    PageTurnLayer();
    virtual ~PageTurnLayer();
    virtual bool init(CCNode* target, PagePos pos);
    static PageTurnLayer* create(CCNode* target, PagePos pos);
    
public:
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    virtual void registerWithTouchDispatcher(void);
    void setTarget(CCNode* target);
    CC_SYNTHESIZE(PageTurn*, m_pTurn, Turn);
    CC_SYNTHESIZE(PagePos, m_epagePos, PagePos);
protected:
    CCNode* m_pSprite;
    bool isInside(CCNode* pNode,CCPoint pos);
    void TurnedCallback();
};
#endif /* defined(__PageTurn__PageTurnLayer__) */
