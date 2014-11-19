#pragma once

#include "Object.h"
#include "Vec2.h"

class Renderer;

class Enemy : public Object
{
public:
    enum Direction { LEFT, RIGHT, UP, DOWN };

    Enemy(Vec2 pos, int width, int height, string filename, float endx);
    ~Enemy();

    Object* clone();
    
    void update(float dt);
    void draw(Renderer* renderer);
    void handleCollision(Object* collider);

    void addVel(float velx, float vely);
    void setVel(float velx, float vely);
    Vec2 getVel();
private:
    float mMoveSpeed;
    float mLeftX, mRightX;
    Direction mDirection;
    float mVelX, mVelY;
};
