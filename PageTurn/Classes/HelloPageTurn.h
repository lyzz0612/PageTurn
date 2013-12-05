//
//  HelloPageTurn.h
//  PageTurn
//
//  Created by lingyun on 13-12-5.
//
//

#ifndef __PageTurn__HelloPageTurn__
#define __PageTurn__HelloPageTurn__

#include <iostream>
#include "cocos2d.h"
#include "PageTurnLayer.h"

USING_NS_CC;

class HelloPageTurn : public CCLayer
{
public:
    HelloPageTurn();
    virtual ~HelloPageTurn();
    virtual bool init();
    CREATE_FUNC(HelloPageTurn);
public:
    static CCScene* scene();
    void TurnedCallback();
};
#endif /* defined(__PageTurn__HelloPageTurn__) */
