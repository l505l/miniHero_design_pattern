#include "dragablesprite.h"

//this whole file was rewriten when implement flyweight pattern 

// dragableSpriteTemplate Implementation
dragableSpriteTemplate* dragableSpriteTemplate::create(const std::string& filename) {
    auto templ = new dragableSpriteTemplate();
    if (templ && templ->init(filename)) {
        templ->autorelease();
        return templ;
    }
    CC_SAFE_DELETE(templ);
    return nullptr;
}

bool dragableSpriteTemplate::init(const std::string& filename) {
    _filename = filename;
    _texture = Director::getInstance()->getTextureCache()->addImage(filename);
    if (_texture) {
        _texture->retain();
        _anchorPoint = Vec2(0.5f, 0.5f);
        return true;
    }
    return false;
}

dragableSpriteTemplate::~dragableSpriteTemplate() {
    CC_SAFE_RELEASE_NULL(_texture);
}

// dragableSpriteFactory Implementation
dragableSpriteFactory* dragableSpriteFactory::_instance = nullptr;

dragableSpriteFactory* dragableSpriteFactory::getInstance() {
    if (!_instance) {
        _instance = new dragableSpriteFactory();
    }
    return _instance;
}

//key refactoring
dragableSpriteTemplate* dragableSpriteFactory::getSpriteTemplate(const std::string& filename) {
    auto it = _templateCache.find(filename);
    if (it != _templateCache.end()) {
        return it->second;
    }

    auto templ = dragableSpriteTemplate::create(filename);
    if (templ) {
        _templateCache[filename] = templ;
        templ->retain();
    }
    return templ;
}
//key refactoring

void dragableSpriteFactory::purgeFactory() {
    for (auto& pair : _templateCache) {
        CC_SAFE_RELEASE_NULL(pair.second);
    }
    _templateCache.clear();
}

dragableSpriteFactory::~dragableSpriteFactory() {
    purgeFactory();
    _instance = nullptr;
}

// dragableSprite Implementation
dragableSprite::dragableSprite()
    : _dragable(false)
    , _dragStart(Vec2::ZERO)
    , _touchOffset(Vec2::ZERO)
    , _presentScene(nullptr)
    , _template(nullptr) {
}

dragableSprite::~dragableSprite() {
}

dragableSprite* dragableSprite::create(const std::string& filename, int tag, Ref* presentScene) {
    auto templ = dragableSpriteFactory::getInstance()->getSpriteTemplate(filename);//key refactoring
    if (!templ) {
        return nullptr;
    }

    auto sprite = new (std::nothrow) dragableSprite();
    if (sprite && sprite->initWithTemplate(templ, tag, presentScene)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool dragableSprite::initWithTemplate(dragableSpriteTemplate* templ, int tag, Ref* presentScene) {
    if (!Sprite::initWithTexture(templ->getTexture())) {//key refactoring
        return false;
    }

    _template = templ;
    setTag(tag);
    setPresentScene(presentScene);
    setDragable(true);
    setAnchorPoint(_template->getAnchorPoint());
    addTouchListener();

    return true;
}

bool dragableSprite::onTouchBegan(Touch* touch, Event* event) {
    setDragStart(this->getPosition());
    _touchOffset = touch->getLocation() - this->getPosition();
    return _dragable && getBoundingBox().containsPoint(touch->getLocation());
}

void dragableSprite::onTouchMoved(Touch* touch, Event* event) {
    this->setPosition(touch->getLocation() - _touchOffset);
}

void dragableSprite::onTouchEnded(Touch* touch, Event* event) {

}

void dragableSprite::addTouchListener() {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(dragableSprite::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(dragableSprite::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(dragableSprite::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}