#include "EnergyBar.h"

EnergyBar* EnergyBar::create(INT32 maxValue, INT32 recoverRate, Ecamp camp) {
    auto bar = new (std::nothrow) EnergyBar();
    if (bar && bar->init(maxValue, recoverRate, camp)) {
        bar->autorelease();
        return bar;
    }
    CC_SAFE_DELETE(bar);
    return nullptr;
}

bool EnergyBar::init(INT32 maxValue, INT32 recoverRate, Ecamp camp) {
    if (!LoadingBar::init()) {
        return false;
    }

    _type = "energy";
    _maxValue = maxValue;
    _currentValue = 0;  // Energy bar starts from 0
    _recoverRate = recoverRate;
    _camp = camp;

    this->loadTexture("slider2.png", TextureResType::LOCAL);
    this->setPercent(0);
    
    // Set energy bar color
    this->setColor(Color3B(39, 123, 221));

    // Add background
    auto background = Sprite::create("slider1.png");
    auto size = getContentSize();
    background->setPosition(size / 2);
    background->setOpacity(200);
    this->addChild(background, -1);

    // Start energy recovery timer
    this->schedule(CC_SCHEDULE_SELECTOR(EnergyBar::recover), 0.5f);

    return true;
}

void EnergyBar::updateValue(INT32 delta) {
    if (_currentValue < 0) {
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

void EnergyBar::recover(float delta) {
    if (_currentValue < _maxValue) {
        updateValue(_recoverRate);
    }
}