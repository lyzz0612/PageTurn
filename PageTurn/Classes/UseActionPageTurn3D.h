//
//  UseActionPageTurn3D.h
//  PageTurn
//
//  Created by lingyun on 13-12-2.
//
//

#ifndef __PageTurn__UseActionPageTurn3D__
#define __PageTurn__UseActionPageTurn3D__

#include <iostream>
#include "cocos2d.h"
#include "CCActionPageTurn3D.h"
#include "PageTurn.h"
USING_NS_CC;


class UseActionPageTurn3D : public CCLayer
{
public:
    UseActionPageTurn3D();
    virtual ~UseActionPageTurn3D();
    static CCScene* scene();
    virtual bool init();
    CREATE_FUNC(UseActionPageTurn3D);
    
public:
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    virtual void registerWithTouchDispatcher(void);
protected:
    CCSprite* m_pSprite;
    PageTurn* m_pTurn;
    bool isInside(CCNode* pNode,CCPoint pos);
};
#endif /* defined(__PageTurn__UseActionPageTurn3D__) */
