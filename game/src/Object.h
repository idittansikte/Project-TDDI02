#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "Vec2.h"
#include "Texture.h"
#include "Renderer.h"

using namespace std;

class Object
{
public:
    enum ObjectId { PLAYER, PLATFORM, ENEMY, POWERUP, LAVA_PLATFORM, FINISH, DECORATION, BACKGROUND };

    Object(Vec2 pos, int width, int height, std::string filename);
    Object(Vec2 pos, int width, int height, Texture* texture, Texture* textureDead);
    virtual ~Object();

    virtual void update(float dt) = 0;
    virtual void draw(Renderer* renderer);
    void draw(Renderer* renderer, Vec2 mousePos);
    virtual void handleCollision(Object* &collider) = 0;
    virtual bool collision(Object* objectA, Object* objectB);
    virtual Object* clone() = 0;
    
    // add get/set
    Vec2 getPosition();
    int getWidth();
    int getHeight();
    void setWidth( int newW) { mWidth = newW; }
    void setHeight( int newH){ mHeight = newH; }
    string getFilename() { return mFilename; }
    void setDead() { mDead = true; }
    void setAlive() { mDead = false; }
    bool getDead() { return mDead; }
    
    void setPosition(Vec2 pos);
    void setPosition(float x, float y);
    void setSize(Vec2 newSize) { mWidth = newSize.x; mHeight = newSize.y; } //FÖR EDITOR
    int getId(){return mId;}
    void setId(int id){ mId = id;}
    Texture* mTexture;
    Texture* mTextureDead;
    void DeadClipCounterReset(){ mDeadClipCounter = 0; }
    bool directionRight{false};
private:
    
    Vec2 mPosition;
    string mFilename;
    int mWidth, mHeight;
    int mId;
    bool mDead;
    int mDeadClipCounter{};
    int mDelayCounter{};
};

#endif
