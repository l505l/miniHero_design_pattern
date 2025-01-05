/*Refactored by Bridge Pattern*/
#include "HealthBar.h"

HealthBar* HealthBar::create(INT32 maxValue, INT32 recoverRate, Ecamp camp) {
    auto bar = new (std::nothrow) HealthBar();
    if (bar && bar->init(maxValue, recoverRate, camp)) {
        bar->autorelease();
        return bar;
    }
    CC_SAFE_DELETE(bar);
    return nullptr;
}

bool HealthBar::init(INT32 maxValue, INT32 recoverRate, Ecamp camp) {
    if (!LoadingBar::init()) {
        return false;
    }

    _type = "health";
    _maxValue = maxValue;
    _currentValue = maxValue;
    _recoverRate = recoverRate;
    _camp = camp;

    this->loadTexture("slider2.png", TextureResType::LOCAL);
    this->setPercent(100);

    // Set health bar color
    if (camp == RED) {
        this->setColor(Color3B(81, 205, 22));
    } else {
        this->setColor(Color3B(255, 0, 0));
    }

    // Add background
    auto background = Sprite::create("slider1.png");
    auto size = getContentSize();
    background->setPosition(size / 2);
    background->setOpacity(200);
    this->addChild(background, -1);

    return true;
}

void HealthBar::updateValue(INT32 delta) {
    if (_currentValue <= 0) {
        return;
    }

    _currentValue += delta;
    
    if (_currentValue > _maxValue) {
        _currentValue = _maxValue;
    } else if (_currentValue < 0) {
        _currentValue = 0;
    }

    updatePercent();
}