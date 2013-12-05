//
//  PageTurn.h
//  PageTurn
//
//  Created by lingyun on 13-12-3.
//
//

#ifndef __PageTurn__PageTurn__
#define __PageTurn__PageTurn__

#include <iostream>
#include "cocos2d.h"
#include "CCActionGrid.h"

enum TouchPos
{
    LeftTop = 0,
    RightTop,
    LeftBottom,
    RightBottom,
};

USING_NS_CC;

class PageTurn : public CCGrid3DAction
{
public:
    PageTurn();
    virtual ~PageTurn();
    static PageTurn* create(float duration, const CCSize& gridsize);
    virtual void update(float time);
    virtual bool isDone();
public:
    void changeShow(const CCPoint& TouchPoint);
    void turnPage();
    void reversePage();
    CCPoint getOriginalPos(CCNode* pNode);
    void registerTurnedCallBack(CCObject* selectortarget, SEL_CallFunc callfunc);
    void unregisterTurnedCallBack(CCObject* selectortarget);
    CC_SYNTHESIZE(TouchPos, m_eTouchPos, TouchPos)
    CC_SYNTHESIZE(bool, m_bisTurned, isTurned);
protected:
    void symShow(float axis);
    CCPoint m_startPos;
    CCPoint m_endPos;
    CCPoint m_eachPos;
protected:
    CCObject* m_pSelectorTargets[20];
    SEL_CallFunc m_pCallFuncs[20];
    int m_nFuncNum;
};
#endif /* defined(__PageTurn__PageTurn__) */
