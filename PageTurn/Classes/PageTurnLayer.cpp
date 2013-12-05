//
//  PageTurnLayer.cpp
//  PageTurn
//
//  Created by lingyun on 13-12-2.
//
//

#include "PageTurnLayer.h"

PageTurnLayer::PageTurnLayer()
{
    m_pSprite = NULL;
    m_pTurn = NULL;
}
PageTurnLayer::~PageTurnLayer()
{
    CC_SAFE_RELEASE_NULL(m_pTurn);
}
PageTurnLayer* PageTurnLayer::create(CCNode *target, PagePos pos)
{
    PageTurnLayer* layer = new PageTurnLayer();
    if(layer && layer->init(target, pos))
    {
        layer->autorelease();
    }
    else
    {
        CC_SAFE_RELEASE_NULL(layer);
    }
    return layer;
}
bool PageTurnLayer::init(CCNode* target, PagePos pos)
{
    if(!CCLayer::init())
        return false;
    
    CCDirector::sharedDirector()->setDepthTest(true);
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    m_pSprite = target;
//    m_pSprite->setAnchorPoint(CCPointZero);
//    m_pSprite->setPosition(ccp(0,0));
    
    m_pTurn = PageTurn::create(10, CCSizeMake(winsize.width / 4, winsize.width / 4));
    m_pTurn->startWithTarget(m_pSprite);
    m_pTurn->retain();
    m_pTurn->registerTurnedCallBack(this, callfunc_selector(PageTurnLayer::TurnedCallback));
    this->addChild(m_pSprite);
    
    setTouchEnabled(true);
    
    m_epagePos = pos;
    return true;
}
void PageTurnLayer::setTarget(CCNode *pTarget)
{
    CC_SAFE_RETAIN(pTarget);
    this->removeChild(m_pSprite);
    m_pSprite = pTarget;
    this->addChild(m_pSprite);
    CC_SAFE_RELEASE(pTarget);
}
bool PageTurnLayer::isInside(CCNode *pNode, CCPoint pos)
{
    return pNode->boundingBox().containsPoint(pos);
}
void PageTurnLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
bool PageTurnLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchpoint = this->convertTouchToNodeSpace(pTouch);
    int flag = 0;
    int temp;
    
    if(isInside(m_pSprite, touchpoint))
    {
        CCPoint oripos = m_pTurn->getOriginalPos(m_pSprite);
        //left
        if(touchpoint.x < oripos.x + m_pSprite->getContentSize().width / 2)
            flag = -1;
        else
            flag = 1;
        
        if(m_epagePos == PAGEPOSLEFT)
            temp = -1;
        else
            temp = 1;
        if(temp * flag < 0)
            return false;
        
        //bottom
        if(touchpoint.y < oripos.y + m_pSprite->getContentSize().height / 2)
            flag += 10;
        else
            flag += 20;
        switch (flag) {
            case 9:
                m_pTurn->setTouchPos(LeftBottom);
                break;
            case 11:
                m_pTurn->setTouchPos(RightBottom);
                break;
            case 19:
                m_pTurn->setTouchPos(LeftTop);
                break;
            case 21:
                m_pTurn->setTouchPos(RightTop);
                break;
            default:
                break;
        }
        
        if( CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(m_pSprite) != 0)
            m_pSprite->stopAction(m_pTurn);
        
        m_pTurn->changeShow(pTouch->getLocation());
        return true;
    }
    return false;
}
void PageTurnLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchpoint = this->convertTouchToNodeSpace(pTouch);
    CCRect rect = m_pSprite->boundingBox();
    if(m_pTurn->getTouchPos() == RightBottom || m_pTurn->getTouchPos() == RightTop)
        rect.setRect(rect.getMinX() - rect.size.width, rect.getMinY(), rect.size.width * 2, rect.size.height);
    else
        rect.setRect(rect.getMinX(), rect.getMinY(), rect.size.width * 2, rect.size.height);
    if(isInside(m_pSprite, touchpoint))
    {
        m_pTurn->changeShow(pTouch->getLocation());
    }
}
void PageTurnLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchpoint = this->convertTouchToNodeSpace(pTouch);
    CCPoint startpoint = this->convertToNodeSpace(pTouch->getStartLocation());
    if(fabsf(startpoint.x - touchpoint.x) >= m_pSprite->getContentSize().width / 3)
    {
        m_pTurn->turnPage();
    }
    else
        m_pTurn->reversePage();
}

void PageTurnLayer::TurnedCallback()
{
    this->setTouchEnabled(false);
}






