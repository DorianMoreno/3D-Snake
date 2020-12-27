#ifndef __SNAKE__H__
#define __SNAKE__H__


#include <chrono>
#include <map>
#include "camera.h"
#include "vector.h"
#include "block.h"
#include <GL/glu.h>

class Snake
{
private:
    // snake reqs:speed; tail
    float s_speed; //(unds/sec)
    float s_SpeedIncrease;
    int s_Size;
    //snake upd neds to know how much time
    std::chrono::time_point<std::chrono::high_resolution_clock> lastPosTime; //time when last pos


    //snake spatial data:: it does behave like a cam
    Vector s_Position;
    Vector s_Forward;
    Vector s_Up;
    Vector s_Right;

    bool s_Grow;
    SnakeBlock* s_Head;
    Vector s_HeadPos;

//nop    Vector s_rots;

public:
    Snake(/* args */);
    Snake(float initialSpeed, float speedIncrease, Vector initialPos, Vector initialForward, Vector initialUp);

    virtual ~Snake();

    float getSpeed();
    Vector getPosition();
    Vector getForward();
    Vector getUp();
    Vector getRight();
    Vector getHeadPos();

    void setPosition(const Vector& position);
    void setForward(const Vector& forward);
    void setUp(const Vector& up);

    void setSpeed(float s);
    bool newHeadPos();
    Vector move();
    Vector currentPos();
    void update(const Vector& position, const Vector& forward, const Vector& up, std::map<Vector, int>& blocks);
    Vector rotate(char direction, float degree = 90);
    Vector turn(const char& direction, const float& rollback = 0.5, const float& nextBox = 0.0);
    void reset(float initialSpeed, Vector initialPos, Vector initialForward, Vector initialUp);
    void increaseSize();

    void drawHead();
    void drawBody();

};

#include "snake.hxx"

#endif
