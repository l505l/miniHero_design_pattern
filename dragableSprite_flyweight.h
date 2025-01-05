/*Refactored by Flyweight Pattern*/
#ifndef __DRAGABLE_SPRITE_H__
#define __DRAGABLE_SPRITE_H__

#include "cocos2d.h"
USING_NS_CC;

// refactoring:shared intrinsics
class dragableSpriteTemplate : public Ref {
private:
    std::string _filename;
    Texture2D* _texture;
    Vec2 _anchorPoint;

public:
    static dragableSpriteTemplate* create(const std::string& filename);
    bool init(const std::string& filename);
    virtual ~dragableSpriteTemplate();

    const std::string& getFilename() const { return _filename; }
    Texture2D* getTexture() const { return _texture; }
    const Vec2& getAnchorPoint() const { return _anchorPoint; }
};

// refactoring:create a factory to manage cache
class dragableSpriteFactory {
private:
    std::unordered_map<std::string, dragableSpriteTemplate*> _templateCache;
    static dragableSpriteFactory* _instance;
    dragableSpriteFactory() {}

public:
    static dragableSpriteFactory* getInstance();
    dragableSpriteTemplate* getSpriteTemplate(const std::string& filename);
    void purgeFactory();
    ~dragableSpriteFactory();
};

// dragableSprite
class dragableSprite : public Sprite {
private:
   
    dragableSpriteTemplate* _template;

    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);

public:
    bool _dragable;
    Vec2 _dragStart;
    Vec2 _touchOffset;
    void addTouchListener();
    Ref* _presentScene;
    dragableSprite();
    virtual ~dragableSprite();

    static dragableSprite* create(const std::string& filename, int tag, Ref* presentScene);
    // refactoring:initialize from shared intrinsics
    bool initWithTemplate(dragableSpriteTemplate* templ, int tag, Ref* presentScene);

    // Getters and setters
    void setDragable(bool dragable) { _dragable = dragable; }
    bool isDragable() const { return _dragable; }
    void setDragStart(const Vec2& pos) { _dragStart = pos; }
    const Vec2& getDragStart() const { return _dragStart; }
    void setPresentScene(Ref* scene) { _presentScene = scene; }
};

#endif