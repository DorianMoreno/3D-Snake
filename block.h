#ifndef __BLOCK__H__
#define __BLOCK__H__

#include <chrono>
#include "vector.h"
#include "mesh.h"
#include <vector>

/**
 */
class Block
{
public:
    //! Memory management
    //@{
    Block(const Vector& colorSolid = Vector(0, 1, 0), const Vector& colorOutline = Vector(1, 0, 0));
    virtual ~Block( );
    //@}

    void draw();

    void setColorSolid(const Vector& colorSolid = Vector(0, 1, 0));
    void setColorOutline(const Vector& colorOutline = Vector(0.5, 1, 0));
    void setPosition(const Vector& position);
    Vector getPosition();


protected:
    Vector position;

    Mesh* solid;
    Mesh* outline;

    void _createSolid(const Vector& colorSolid = Vector(0, 1, 0));
    void _createOutline(const Vector& colorOutline = Vector(0.5, 1, 0));
};

class SnakeBlock: public Block
{
public:
    SnakeBlock(const Vector& initPos = Vector(0, 0, 0));
    ~SnakeBlock();

    Vector advance(const Vector& newPosition);
    void increaseSize(const Vector& newPosition);
    void draw();
    void drawHead();
    void drawFull();
    void drawBody();
protected:
    SnakeBlock* nextSnakeBlock;
};

class FoodBlock: public Block
{
public:
    FoodBlock(const int& worldSize, const Vector& initPos = Vector(0, 0, 0));
    ~FoodBlock();

    void draw();
protected:
    Mesh* light;

    void _createLight(const float& worldSize);
};



#include "block.hxx"

#endif // __BLOCK__H__

// eof - block.h
