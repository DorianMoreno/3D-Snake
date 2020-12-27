#ifndef __SNAKE__HXX__
#define __SNAKE__HXX__

#include "snake.h"
#include <cmath>
#include <GL/glu.h>
#include <iostream>

#define threshold 0.01

Snake::
Snake(float initialSpeed, float speedIncrease, Vector initialPos, Vector initialForward, Vector initialUp){

    this->s_speed         = initialSpeed;
    this->s_SpeedIncrease = speedIncrease;
    this->s_Size          = 1;
    this->s_Position      = initialPos;
    this->s_Forward       = initialForward;
    this->s_Up            = initialUp;
    this->s_Right         = s_Forward*s_Up;
    this->s_Head          = new SnakeBlock(initialPos);
    this->s_Grow          = false;
    this->s_HeadPos       = initialPos - Vector(0.5, 0.5, 0.5);
    this->lastPosTime     = std::chrono::high_resolution_clock::now();
}

Snake::
~Snake()
{
    if(this->s_Head != nullptr){
        delete(this->s_Head);
        this->s_Head = nullptr;
    }
}

bool Snake::
newHeadPos()
{
    Vector actualBox = this->s_Position;
    actualBox[0] = (int)(actualBox[0]);
    actualBox[1] = (int)(actualBox[1]);
    actualBox[2] = (int)(actualBox[2]);
    actualBox -= this->s_HeadPos;

    if(actualBox.getNorm() > threshold)
        return true;
    return false;
}

Vector Snake::
move()
{
    Vector actualBox = this->s_Position;
    actualBox[0] = (int)(actualBox[0]);
    actualBox[1] = (int)(actualBox[1]);
    actualBox[2] = (int)(actualBox[2]);

    this->s_HeadPos = actualBox;
    if(this->s_Grow){
        this->s_Head->increaseSize(actualBox);
        this->s_Grow = false;
        return Vector(-1, -1, -1);
    }
    return this->s_Head->advance(actualBox);
}

Vector Snake::
currentPos(){
    float dx;
    double dt =
        std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now( ) - this->lastPosTime
        ).count( );
    dt /= 1000; //from mill to sec
    this->lastPosTime = std::chrono::high_resolution_clock::now();
    this->s_Position += (this->s_Forward * this->s_speed * dt);
    return this->s_Position;
}

Vector Snake::
turn(const char& dir, const float& rollback, const float& nextBox){
    this->s_Position += this->s_Forward*nextBox;
    this->s_Position[0] = (int)(this->s_Position[0]);
    this->s_Position[0] += 0.5;
    this->s_Position[1] = (int)(this->s_Position[1]);
    this->s_Position[1] += 0.5;
    this->s_Position[2] = (int)(this->s_Position[2]);
    this->s_Position[2] += 0.5;

    Vector copyForward = this->s_Forward;
    switch (dir)
    {
    case 'u':
        this->s_Forward = this->s_Up;

        this->s_Up = copyForward * -1.0;
        this->s_Position += this->s_Forward*0.5;
        this->s_Position += this->s_Forward*-rollback;
        return this->s_Right;

    case 'd':
        this->s_Forward = this->s_Up * -1.0;

        this->s_Up = copyForward;
        this->s_Position += this->s_Forward*0.5;
        this->s_Position += this->s_Forward*-rollback;
        return this->s_Right * -1.0;

    case 'r':
        this->s_Forward = this->s_Right;

        this->s_Right = copyForward * -1.0;
        this->s_Position += this->s_Forward*0.5;
        this->s_Position += this->s_Forward*-rollback;
        return this->s_Up * -1.0;

    case 'l':
        this->s_Forward = this->s_Right * -1.0;

        this->s_Right = copyForward;
        this->s_Position += this->s_Forward*0.5;
        this->s_Position += this->s_Forward*-rollback;
        return this->s_Up;

    default:
        return Vector(0, 0, 0);
    }
}


void Snake::
setSpeed(float s){
    this->s_speed = s;
}

float Snake::
getSpeed(){
    return this->s_speed;
}

void Snake::
setForward(const Vector& forward){
    this->s_Forward = forward;
    this->s_Right = this->s_Forward*this->s_Up;
}

Vector Snake::
getForward(){
    return this->s_Forward;
}

void Snake::
setUp(const Vector& up){
    this->s_Up = up;
    this->s_Right = this->s_Forward*this->s_Up;
}

Vector Snake::
getUp(){
    return this->s_Up;
}

void Snake::
setPosition(const Vector& position){
    this->s_Position = position;
}

Vector Snake::
getPosition(){
    return this->s_Position;
}

Vector Snake::
getRight(){
    return this->s_Right;
}

Vector Snake::
getHeadPos(){
    return this->s_HeadPos;
}

Vector Snake::
update(const Vector& position, const Vector& forward, const Vector& up, std::map<Vector, int>& blocks)
{
    this->s_Position    = position;
    this->s_Forward     = forward;
    this->s_Up          = up;

    if(this->newHeadPos() == true){
        if(this->s_Size > 1)
            blocks[this->s_HeadPos] = 0;
        Vector toRemove = this->move();
        if(toRemove[0] != -1.0)
            blocks.erase(toRemove);
    }
}

void Snake::
increaseSize()
{
    if(!this->s_Grow){
        this->s_Size++;
        this->s_speed = std::min(this->s_SpeedIncrease + this->s_speed, 5.0f);
    }
    this->s_Grow = true;
}

void Snake::
reset(float initialSpeed, Vector initialPos, Vector initialForward, Vector initialUp)
{
    if(this->s_Head != nullptr)
        delete this->s_Head;

    this->s_speed       = initialSpeed;
    this->s_Size        = 1;
    this->s_Position    = initialPos;
    this->s_Forward     = initialForward;
    this->s_Up          = initialUp;
    this->s_Right       = s_Forward*s_Up;
    this->s_Head        = new SnakeBlock(initialPos);
    this->s_Grow        = false;
    this->s_HeadPos     = initialPos - Vector(0.5, 0.5, 0.5);
    this->lastPosTime   = std::chrono::high_resolution_clock::now();
}

void Snake::
drawHead()
{
    this->s_Head->drawHead();
}

void Snake::
drawBody()
{
    this->s_Head->drawBody();
}

#endif // __SNAKE__HXX__
