//
//  Block.hpp
//  Arkanoid
//
//  Created by Alexander Bekert on 24/07/16.
//
//

#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Block;

class BlockStatesDelegate {
public:
    virtual void blockChangedState(Block *block) { CCLOG("New Block State"); }
};

class Block : public CCNode
{
public:
    static float destroyAnimationDuration;
    CREATE_FUNC(Block);
    static Block *create(float width, float height);
    static Block *create(float width, float height, ccColor3B color);
    void setVisible();
    void setVisible(bool visible);
    void setVisible(float delay, bool visible = true);
    
    bool takeDamage();
    
    void fadeOut(float duration);
    CCSize getContentSize() { return size; }
    
    typedef enum {
        StateActive,
        StateInactive,
        StateDestroyedPlayingAnimation,
        StateDestroyed
    } State;
    State getState() { return state; }
    
    BlockStatesDelegate *delegate;
    
private:
    CCSprite *sprite;
    CCSize size;
    State state;
    
    void setStateDestroyed() {
        setState(StateDestroyed);
        sprite->removeFromParent();
    }
    void setState(State newState) {
        if (state == newState) return;
        state = newState;
        if (delegate) delegate->blockChangedState(this);
    }
};