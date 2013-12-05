//
//  PageTurn.cpp
//  PageTurn
//
//  Created by lingyun on 13-12-3.
//
//

#include "PageTurn.h"

PageTurn::PageTurn()
{
    m_bisTurned = false;
    m_nFuncNum = 0;
}
PageTurn::~PageTurn()
{
    
}

PageTurn* PageTurn::create(float duration, const CCSize &gridsize)
{
    PageTurn* ret = new PageTurn();
    if(ret && ret->initWithDuration(duration, gridsize))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE_NULL(ret);
    return NULL;
}
bool PageTurn::isDone()
{
    return false;
}
void PageTurn::registerTurnedCallBack(CCObject* selectortarget, SEL_CallFunc callfunc)
{
    for(int i = 0;i < m_nFuncNum;i++)
    {
        if(m_pSelectorTargets[i] == selectortarget)
        {
            CCLog("Target already has a callfunction now!");
            return;
        }
    }
    CC_SAFE_RETAIN(selectortarget);
    m_pSelectorTargets[m_nFuncNum] = selectortarget;
    m_pCallFuncs[m_nFuncNum++] = callfunc;
}
void PageTurn::unregisterTurnedCallBack(CCObject *selectortarget)
{
    int i,j;
    for(i = 0;i < m_nFuncNum;i++)
    {
        if(m_pSelectorTargets[i] == selectortarget)
            break;
    }
    if(i == m_nFuncNum)
        return;
    CC_SAFE_RELEASE(m_pSelectorTargets[i]);
    
    for(j = i;j < m_nFuncNum - 1;j++)
    {
        m_pSelectorTargets[j] = m_pSelectorTargets[j+1];
        m_pCallFuncs[j] = m_pCallFuncs[j+1];
    }
    m_nFuncNum--;
}
void PageTurn::update(float time)
{
    m_startPos = m_startPos + m_eachPos;
    changeShow(m_startPos);
    CCPoint oripos = getOriginalPos(m_pTarget);
    CCPoint world_ori = m_pTarget->getParent()->convertToWorldSpace(oripos);
    if(m_startPos.y <= world_ori.y)
    {
        m_pTarget->stopAction(this);
        if(m_bisTurned && m_nFuncNum > 0)
        {
            for (int i = 0; i < m_nFuncNum; i++) {
                (m_pSelectorTargets[i]->*m_pCallFuncs[i])();
            }
            
        }
        return;
    }
}
CCPoint PageTurn::getOriginalPos(CCNode *pNode)
{
    CCPoint anspos = pNode->getAnchorPoint();
    CCPoint pos = pNode->getPosition();
    CCSize size = pNode->getContentSize();
    
    CCPoint realpos;
    realpos.x = pos.x - anspos.x * size.width;
    realpos.y = pos.y - anspos.y * size.height;
    return realpos;
}

//TouchPoint是屏幕world坐标
void PageTurn::changeShow(const CCPoint &TouchPoint)
{
    CCPoint oripos = getOriginalPos(m_pTarget);
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    CCSize targetSize = m_pTarget->getContentSize();
    CCPoint worldOripos = m_pTarget->getParent()->convertToWorldSpace(oripos);
    
    int minx = worldOripos.x / winsize.width * m_sGridSize.width;
    int maxx = (worldOripos.x + targetSize.width) / winsize.width * m_sGridSize.width;
    int miny = worldOripos.y / winsize.height * m_sGridSize.height;
    int maxy = (worldOripos.y + targetSize.height) / winsize.height * m_sGridSize.height;
    
    //以target起始点作为原点计算
    //http://design.yesky.com/flash/257/2566757.shtml 顶点命名以网页中图所示
    CCPoint vertex_C = CCPointMake(TouchPoint.x - worldOripos.x, TouchPoint.y - worldOripos.y);
    CCPoint vertex_H;
    switch (m_eTouchPos) {
        case RightBottom:
            vertex_H = CCPointMake(targetSize.width, 0);
            break;
        case LeftBottom:
            vertex_H = CCPointMake(0, 0);
            break;
        case RightTop:
            vertex_H = CCPointMake(targetSize.width, targetSize.height);
            break;
        case LeftTop:
            vertex_H = CCPointMake(0, targetSize.height);
            break;
        default:
            break;
    }
    
    if(vertex_C.x == vertex_H.x)
    {
        CCLog("No to turn Page");
        return;
    }
    if(vertex_C.y == vertex_H.y)
    {
        symShow((vertex_C.x + vertex_H.x) / 2);
        return;
    }
    float line_CH_k = (vertex_H.y - vertex_C.y) / (vertex_H.x - vertex_C.x);
//    float line_CH_b = ( (vertex_H.y * vertex_C.x) - (vertex_C.y * vertex_H.x) ) / (vertex_H.x - vertex_C.x);
    
    float line_BD_k = 0.0f - 1.0f / line_CH_k;
    float line_BD_b = ((vertex_H.y + vertex_C.y) / 2) - line_BD_k * ((vertex_H.x + vertex_C.x) / 2);
    
    CCPoint vertex_D;
    CCPoint vertex_B;
    float temp;
    //d是bd与横线的交点
    if(m_eTouchPos == LeftBottom || m_eTouchPos == RightBottom)
    {
        temp = -line_BD_b / line_BD_k;
        if(temp < 0)
            temp = 0;
        if(temp > targetSize.width)
            temp = targetSize.width;
        line_BD_b = -temp * line_BD_k;
    }
    else
    {
        temp = (targetSize.height - line_BD_b) / line_BD_k;
        if(temp < 0)
            temp = 0;
        if(temp > targetSize.width)
            temp = targetSize.width;
        line_BD_b = targetSize.height - temp * line_BD_k;
    }
    
    CCPoint vertex_sym;
    float sym_z;
    ccVertex3F p;
    for(int i = minx; i <= maxx + 1;++i)
    {
        for(int j = miny; j <= maxy + 1;++j)
        {
            //屏幕坐标
            p = originalVertex(ccp(i ,j));
            p.x -= worldOripos.x;
            p.y -= worldOripos.y;
            
            if(p.y - line_BD_k * p.x - line_BD_b >= 0)
            {
                if(m_eTouchPos == LeftBottom || m_eTouchPos == RightBottom)
                    continue;
            }
            else
            {
                if(m_eTouchPos == LeftTop || m_eTouchPos == RightTop)
                    continue;
            }
            vertex_sym.x = p.x - (2 * line_BD_k * (line_BD_k * p.x - p.y + line_BD_b)) / (line_BD_k * line_BD_k + 1);
            vertex_sym.y = p.y - (-2 * (line_BD_k * p.x - p.y + line_BD_b)) / (line_BD_k * line_BD_k + 1);
            
            
            sym_z = fabsf(line_BD_k * p.x - p.y + line_BD_b) / (line_BD_k * line_BD_k + 1) /  targetSize.width * 2.0f;
            
            p.x = worldOripos.x + vertex_sym.x;
            p.y = worldOripos.y + vertex_sym.y;
            p.z = sym_z;
            setVertex(ccp(i, j), p);
        }
    }
        
}
void PageTurn::symShow(float axis)
{
    CCPoint oripos = getOriginalPos(m_pTarget);
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    CCSize targetSize = m_pTarget->getContentSize();
    CCPoint worldOripos = m_pTarget->getParent()->convertToWorldSpace(oripos);
    
    if(axis > targetSize.width)
    {
        m_pTarget->stopAction(this);
        return;
    }
    int minx = worldOripos.x / winsize.width * m_sGridSize.width;
    int maxx = (worldOripos.x + targetSize.width) / winsize.width * m_sGridSize.width;
    int miny = worldOripos.y / winsize.height * m_sGridSize.height;
    int maxy = (worldOripos.y + targetSize.height) / winsize.height * m_sGridSize.height;

    ccVertex3F p;
    for(int i = minx;i <= maxx;++i)
    {
        p = originalVertex(ccp(i, 0));
        if(p.x - worldOripos.x <= axis)
            continue;
        for (int j = miny; j <= maxy; ++j)
        {
            p = originalVertex(ccp(i, j));
            p.x -= worldOripos.x;
            p.x = 2 * axis - p.x + worldOripos.x;
            setVertex(ccp(i, j), p);
        }
    }
}
//屏幕坐标
void PageTurn::turnPage()
{
    CCPoint oripos = getOriginalPos(m_pTarget);
    CCPoint world_ori = m_pTarget->getParent()->convertToWorldSpace(oripos);
    CCSize size = m_pTarget->getContentSize();
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    int minx = world_ori.x / winsize.width * m_sGridSize.width;
    int maxx = (world_ori.x + size.width) / winsize.width * m_sGridSize.width;
    int miny = world_ori.y / winsize.height * m_sGridSize.height;
    int maxy = (world_ori.y + size.height) / winsize.height * m_sGridSize.height;
    
    switch (m_eTouchPos) {
        case RightBottom:
            m_endPos = CCPointMake(world_ori.x - size.width, world_ori.y);
            m_startPos = CCPointMake(maxx, miny);
            break;
        case LeftBottom:
            m_endPos = CCPointMake(world_ori.x + size.width * 2, world_ori.y);
            m_startPos = CCPointMake(minx, miny);
            break;
        case RightTop:
            m_endPos = CCPointMake(world_ori.x - size.width, world_ori.y + size.height);
            m_startPos = CCPointMake(maxx, maxy);
            break;
        case LeftTop:
            m_endPos = CCPointMake(world_ori.x + size.width * 2, world_ori.y + size.height);
            m_startPos = CCPointMake(minx, maxy);
            break;
        default:
            break;
    }
    ccVertex3F temp = vertex(m_startPos);
    m_startPos = CCPointMake(temp.x, temp.y);
    
    float time = m_startPos.getDistance(m_endPos) / 500;
    m_eachPos = CCPointMake( (m_endPos.x - m_startPos.x) / 60 / time, (m_endPos.y - m_startPos.y) / 60 / time);
    m_pTarget->stopAction(this);
    m_bisTurned = true;
    m_pTarget->runAction(this);
}
//屏幕坐标
void PageTurn::reversePage()
{
    CCPoint oripos = getOriginalPos(m_pTarget);
    CCPoint world_ori = m_pTarget->getParent()->convertToWorldSpace(oripos);
    CCSize size = m_pTarget->getContentSize();
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    int minx = world_ori.x / winsize.width * m_sGridSize.width;
    int maxx = (world_ori.x + size.width) / winsize.width * m_sGridSize.width;
    int miny = world_ori.y / winsize.height * m_sGridSize.height;
    int maxy = (world_ori.y + size.height) / winsize.height * m_sGridSize.height;
    
    switch (m_eTouchPos) {
        case RightBottom:
            m_endPos = CCPointMake(world_ori.x + size.width, world_ori.y);
            m_startPos = CCPointMake(maxx, miny);
            break;
        case LeftBottom:
            m_endPos = CCPointMake(world_ori.x, world_ori.y);
            m_startPos = CCPointMake(minx, miny);
            break;
        case RightTop:
            m_endPos = CCPointMake(world_ori.x + size.width, world_ori.y + size.height);
            m_startPos = CCPointMake(maxx, maxy);
            break;
        case LeftTop:
            m_endPos = CCPointMake(world_ori.x, world_ori.y + size.height);
            m_startPos = CCPointMake(minx, maxy);
            break;
        default:
            break;
    }
    ccVertex3F temp = vertex(m_startPos);
    m_startPos = CCPointMake(temp.x, temp.y);
    
    float time = m_startPos.getDistance(CCPointMake(world_ori.x + size.width, world_ori.y)) / 500 + 0.5;
    m_eachPos = CCPointMake( (m_endPos.x - m_startPos.x) / 60 / time, (m_endPos.y - m_startPos.y) / 60 / time);
    m_pTarget->stopAction(this);
    m_pTarget->runAction(this);
}








