#include "Player.h"
#include <iostream>

Player::Player(Vec2 pos, int width, int height, string filename)
    : Object(pos, width, height, filename)
{
    mVelY = 0;
    mVelX = 0;
    setId(0);
}

Player::~Player()
{

}

void Player::update(float dt)
{
    //Updaterar pos efter mVel från PlayState
    //setPosition(getPosition().x + mVelX, getPosition().y + mVelY);
    
}

void Player::draw(Renderer* renderer)
{
    Object::draw(renderer);
}

void Player::handleCollision(Object* object)
{
    /* 
    //Move the dot left or right
    setPosition(getPosition().x + mVelX, getPosition().y);
	//mCollider.x = mPosX;

    //If the dot collided or went too far to the left or right
    if( collision(this, object) )
    {
        //Move back
        setPosition(getPosition().x - mVelX, getPosition().y);
		//mCollider.x = mPosX;
    }

    //Move the dot up or down
    setPosition(getPosition().x, getPosition().y + mVelY);
	//mCollider.y = mPosY;

    //If the dot collided or went too far up or down
    if( collision(this, object) )
    {
        //Move back
        setPosition(getPosition().x, getPosition().y - mVelY);
		//mCollider.y = mPosY;
    }
    
    return cDetected; */
}

void Player::addVel(int VelX, int VelY)
{
    mVelX += VelX;
    mVelY += VelY;
}

/*void Player::playerJump(int speed)
{
    if(jumpEnable)
    {
        
    }
}*/

Vec2 Player::getVel()
{
    return Vec2(mVelX, mVelY);
}

Object* Player::clone()
{
    Object* NewObject = new Player(getPosition(), getWidth(), getHeight(), getFilename());
    return NewObject;
}

void Player::setjump(int i)
{
    if(i==1)
    {
	jumping = true;
    }
    else
	jumping = false;
}

void Player::setfall(int i)
{
    if(i==1)
    {
	falling = true;
	mVelY = 2;
	cout << "Falling = true" << endl;
    }
    else
    {
	falling = false;
	cout << "Falling = false" << endl;
    }
}

bool Player::getjump()
{
    return jumping;
}

bool Player::getfall()
{
    return falling;
}