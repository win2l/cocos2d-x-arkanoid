//
//  Raquet.cpp
//  Arkanoid
//
//  Created by Alexander Bekert on 29/07/16.
//
//

#include "Raquet.hpp"

#include "../../Extensions/ColorSprite.hpp"

static const int colorizeActionTag = 1;

Raquet* Raquet::create(float width, float height) {
    return create(width, height, ccWHITE);
}

Raquet* Raquet::create(float width, float height, ccColor3B color) {
    auto raquet = new Raquet();
    if (raquet && raquet->init()) {
        raquet->autorelease();
        auto color4b = ccc4(color.r, color.g, color.b, 255);
        auto size = CCSize(width, height);
        raquet->sprite = ColorSprite::create(color4b, size);
        raquet->addChild(raquet->sprite);
        raquet->size = size;
        raquet->updateBordersX();
        CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(raquet, 0, false);
        
        return raquet;
    }
    
    delete raquet;
    raquet = NULL;
    return NULL;
}

Raquet::~Raquet() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
}

void Raquet::handleTouchAtPosition(CCPoint point) {
    desiredPoint = ccp(point.x, y);
}

void Raquet::setY(float newY) {
    y = newY;
    setPosition(ccp(getPositionX(), y));
}

void Raquet::update(float dt) {
    float x = getPositionX();
    if (x > desiredPoint.x) {
        x -= speedX * dt;
        x = MAX(x, desiredPoint.x);
    } else if (x < desiredPoint.x) {
        x += speedX * dt;
        x = MIN(x, desiredPoint.x);
    } else {
        x = desiredPoint.x;
    }
    x = clampf(x, xMin, xMax);
    setPosition(ccp(x, desiredPoint.y));
}

void Raquet::updateBordersX() {
    float width = sprite->getContentSize().width;
    float screenWidth = CCDirector::sharedDirector()->getVisibleSize().width;
    
    xMin = width * 0.5f;
    xMax = screenWidth - xMin;
    
    fitPositionIntoBordersX();
}

void Raquet::fitPositionIntoBordersX() {
    float x = clampf(getPositionX(), xMin, xMax);
    setPositionX(x);
}

float Raquet::reflectionForcePercentX(CCPoint ballPosition) {
    auto x = convertToNodeSpace(ballPosition).x;
    return clampf(2 * x / size.width, -1, 1);
}

void Raquet::moveToStartPosition() {
    desiredPoint.x = (xMin + xMax) / 2;
}

#pragma mark Color

void Raquet::setColor(ccColor3B color) {
    sprite->setColor(color);
}

void Raquet::setColor(ccColor3B color, float duration) {
    auto colorize = CCTintTo::create(duration, color.r, color.g, color.b);
    colorize->setTag(colorizeActionTag);
    sprite->stopActionByTag(colorizeActionTag);
    sprite->runAction(colorize);
}

