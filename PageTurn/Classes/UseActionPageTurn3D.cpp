//
//  UseActionPageTurn3D.cpp
//  PageTurn
//
//  Created by lingyun on 13-12-2.
//
//

#include "UseActionPageTurn3D.h"

UseActionPageTurn3D::UseActionPageTurn3D()
{
    m_pSprite = NULL;
    m_pTurn = NULL;
}
UseActionPageTurn3D::~UseActionPageTurn3D()
{
    CC_SAFE_RELEASE_NULL(m_pTurn);
}

CCScene* UseActionPageTurn3D::scene()
{
    CCScene* scene = CCScene::create();
    UseActionPageTurn3D* layer = UseActionPageTurn3D::create();
    layer->setAnchorPoint(CCPointZero);
    layer->setPosition(CCPointZero);
    scene->addChild(layer);
    return scene;
}

bool UseActionPageTurn3D::init()
{
    if(!CCLayer::init())
        return false;
    
    CCDirector::sharedDirector()->setDepthTest(true);
    m_pSprite = CCSprite::create("test1.png");
    m_pSprite->setAnchorPoint(CCPointZero);
    m_pSprite->setPosition(ccp(300,20));
    
    m_pTurn = PageTurn::create(3, CCSizeMake(240, 180));
    m_pTurn->startWithTarget(m_pSprite);
    m_pTurn->retain();
    
    this->addChild(m_pSprite,2);
    
    setTouchEnabled(true);
    return true;
}
bool UseActionPageTurn3D::isInside(CCNode *pNode, CCPoint pos)
{
    return pNode->boundingBox().containsPoint(pos);
}
void UseActionPageTurn3D::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
bool UseActionPageTurn3D::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchpoint = this->convertTouchToNodeSpace(pTouch);
    int flag = 0;
    if(isInside(m_pSprite, touchpoint))
    {
        CCPoint oripos = m_pTurn->getOriginalPos(m_pSprite);
        //left
        if(touchpoint.x < oripos.x + m_pSprite->getContentSize().width / 2)
            flag += 1;
        else
            flag += 2;
        //bottom
        if(touchpoint.y < oripos.y + m_pSprite->getContentSize().height / 2)
            flag += 10;
        else
            flag += 20;
        switch (flag) {
            case 11:
                m_pTurn->setTouchPos(LeftBottom);
                break;
            case 12:
                m_pTurn->setTouchPos(RightBottom);
                break;
            case 21:
                m_pTurn->setTouchPos(LeftTop);
                break;
            case 22:
                m_pTurn->setTouchPos(RightTop);
                break;
            default:
                break;
        }
        m_pSprite->stopAction(m_pTurn);
        m_pTurn->changeShow(pTouch->getLocation());
        return true;
    }
    return false;
}
void UseActionPageTurn3D::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchpoint = this->convertTouchToNodeSpace(pTouch);
    if(isInside(m_pSprite, touchpoint))
    {
        m_pTurn->changeShow(pTouch->getLocation());
    }
}
void UseActionPageTurn3D::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
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








