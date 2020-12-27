#ifndef __BLOCK__HXX__
#define __BLOCK__HXX__

#include "block.h"
#include <cmath>
#include <GL/glu.h>

Block::
Block(const Vector& colorSolid, const Vector& colorOutline):
    solid(nullptr),
    outline(nullptr)
{
    _createSolid(colorSolid);
    _createOutline(colorOutline);
}

Block::
~Block( )
{
    if(this->solid != nullptr){
        delete(this->solid);
        this->solid = nullptr;
    }
    if(this->outline != nullptr){
        delete(this->outline);
        this->outline = nullptr;
    }
}

void Block::
draw()
{
    if(this->solid == nullptr)
    return;
    if(this->outline == nullptr)
    return;

    glPushMatrix();
        glTranslatef(this->position[0], this->position[1], this->position[2]);
        this->solid->drawInOpenGLContext(GL_POLYGON);
    glPopMatrix();
}

void Block::
setColorSolid(const Vector& colorSolid)
{
    if(this->solid == nullptr)
        return;
    this->solid->setColor(colorSolid[0], colorSolid[1], colorSolid[2]);
}

void Block::
setColorOutline(const Vector& colorOutline)
{
    if(this->outline == nullptr)
    return;
    this->outline->setColor(colorOutline[0], colorOutline[1], colorOutline[2]);
}

void Block::
setPosition(const Vector& position)
{
    this->position = position;
}

Vector Block::
getPosition()
{
    return this->position;
}


void Block::
_createSolid(const Vector& colorSolid)
{
    if(this->solid != nullptr){
        delete(this->solid);
        this->solid = nullptr;
    }
    this->solid = new Mesh("triangleCube.obj");
    this->solid->setColor(colorSolid[0], colorSolid[1], colorSolid[2]);
}

void Block::
_createOutline(const Vector& colorOutline)
{
    if(this->outline != nullptr){
        delete(this->outline);
        this->outline = nullptr;
    }
    this->outline = new Mesh("cube.obj");
    this->outline->setColor(colorOutline[0], colorOutline[1], colorOutline[2]);
}


SnakeBlock::
SnakeBlock(const Vector& initPos):
    nextSnakeBlock(nullptr)
{
    _createSolid(Vector(0.9, 0.9, 0.9));
    _createOutline(Vector(0, 0.6, 0));

    this->position = initPos;
}

SnakeBlock::
~SnakeBlock()
{
    if(this->solid != nullptr){
        delete(this->solid);
        this->solid = nullptr;
    }
    if(this->outline != nullptr){
        delete(this->outline);
        this->outline = nullptr;
    }
    if(this->nextSnakeBlock != nullptr){
        delete(this->nextSnakeBlock);
        this->nextSnakeBlock = nullptr;
    }
}

Vector SnakeBlock::
advance(const Vector& newPosition)
{
    Vector oldPosition = this->position;
    this->position = newPosition;
    if(this->nextSnakeBlock == nullptr)
        return oldPosition;
    return this->nextSnakeBlock->advance(oldPosition);
}

void SnakeBlock::
increaseSize(const Vector& newPosition)
{
    Vector oldPosition = this->position;
    this->position = newPosition;
    if(this->nextSnakeBlock == nullptr){
        this->nextSnakeBlock = new SnakeBlock(oldPosition);
        return;
    }
    this->nextSnakeBlock->increaseSize(oldPosition);
}

void SnakeBlock::
draw()
{
    if(this->solid == nullptr)
    return;
    if(this->outline == nullptr)
    return;

    glPushMatrix();
        glTranslatef(this->position[0], this->position[1], this->position[2]);
        this->solid->drawInOpenGLContext(GL_POLYGON);
    glPopMatrix();
}

void SnakeBlock::
drawHead()
{
    this->draw();
    if (this->nextSnakeBlock != nullptr)
        this->nextSnakeBlock->draw();
}

void SnakeBlock::
drawFull()
{
    if(this->solid == nullptr)
        return;
    if(this->outline == nullptr)
        return;

    glPushMatrix();
        glTranslatef(this->position[0], this->position[1], this->position[2]);
        this->solid->drawInOpenGLContext(GL_POLYGON);
        // this->outline->drawInOpenGLContext(GL_LINE_LOOP);
    glPopMatrix();
    if(this->nextSnakeBlock == nullptr){
        return;
    }
    this->nextSnakeBlock->drawFull();
}

void SnakeBlock::
drawBody()
{
    if(this->nextSnakeBlock == nullptr || this->nextSnakeBlock->nextSnakeBlock == nullptr)
        return;
    this->nextSnakeBlock->nextSnakeBlock->drawFull();
}

FoodBlock::
FoodBlock(const int& worldSize, const Vector& initPos):
    light(nullptr)
{
    _createSolid(Vector(1, 0.3, 1));
    _createOutline(Vector(0.6, 0, 0));
    _createLight(worldSize);

    this->position = initPos;
}

FoodBlock::
~FoodBlock()
{
    if(this->solid != nullptr){
        delete(this->solid);
        this->solid = nullptr;
    }
    if(this->outline != nullptr){
        delete(this->outline);
        this->outline = nullptr;
    }
    if(this->light != nullptr){
        delete(this->light);
        this->light = nullptr;
    }
}

void FoodBlock::
draw()
{
    if(this->solid == nullptr)
        return;
    if(this->outline == nullptr)
        return;
    if(this->light == nullptr)
        return;

    glPushMatrix();
        glTranslatef(this->position[0], this->position[1], this->position[2]);
        this->solid->drawInOpenGLContext(GL_POLYGON);
        // this->outline->drawInOpenGLContext(GL_LINE_LOOP);
    glPopMatrix();
}

void FoodBlock::
_createLight(const float& worldSize)
{
    if(this->light == nullptr)
        delete this->light;
    this->light = new Mesh();

    this->light->addPoint( worldSize/2.0,            0.5,            0.5);
    this->light->addPoint(-worldSize/2.0,            0.5,            0.5);
    this->light->addPoint(           0.5,  worldSize/2.0,            0.5);
    this->light->addPoint(           0.5, -worldSize/2.0,            0.5);
    this->light->addPoint(           0.5,            0.5,  worldSize/2.0);
    this->light->addPoint(           0.5,            0.5, -worldSize/2.0);

    std::vector<unsigned int> face1(2);
    std::vector<unsigned int> face2(2);
    std::vector<unsigned int> face3(2);
	face1[0] = 1; face1[1] = 2;
	face2[0] = 3; face2[1] = 4;
	face3[0] = 5; face3[1] = 6;

	this->light->addFace( face1 );
	this->light->addFace( face2 );
	this->light->addFace( face3 );

    this->light->setColor(1, 0, 0);
}

#endif //__BLOCK__HXX__
// eof - block.hxx
