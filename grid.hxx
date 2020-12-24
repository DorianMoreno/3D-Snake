#ifndef __GRID__HXX__
#define __GRID__HXX__

#include "grid.h"
#include "vector.h"
#include "quaternion.h"
#include "mesh.h"
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

//------------------------------------------------------------------------------
#define _MIN_TIME   0.01
#define _THRESHOLD  0.01
#define _GRAVITY    100
#define _GRAVITY_2  50

//------------------------------------------------------------------------------
Grid::Grid():
	boxNumber(0),
	corner(false),
	edge(nullptr),
	grid(nullptr)
{

}

Grid::Grid(const int& boxNumber, const Vector& corner):
	boxNumber(0),
	corner(false),
	edge(nullptr),
	grid(nullptr)
{
	this->boxNumber = boxNumber;
	this->corner = corner;

	this->_createEdge();
	this->_createGrid();
}

Grid::Grid(const int& boxNumber, const float& x, const float& y, const float& z):
	boxNumber(0),
	corner(false),
	edge(nullptr),
	grid(nullptr)
{
	this->boxNumber = boxNumber;
	this->corner = Vector(x, y, z);

	this->_createEdge();
	this->_createGrid();
}

Grid::~Grid(){
	if(this->edge != nullptr)
		delete this->edge;
	if(this->grid != nullptr)
		delete this->grid;
}

void Grid::_createEdge(){
	if( this->edge != nullptr )
		delete this->edge;
	this->edge = new Mesh();

	this->edge->addPoint( 0.5,  0.5,  0.5); //Point 1
	this->edge->addPoint( 0.5, -0.5,  0.5); //Point 2
	this->edge->addPoint( 0.5, -0.5, -0.5); //Point 3
	this->edge->addPoint( 0.5,  0.5, -0.5); //Point 4

	this->edge->addPoint(-0.5,  0.5,  0.5); //Point 5
	this->edge->addPoint(-0.5, -0.5,  0.5); //Point 6
	this->edge->addPoint(-0.5, -0.5, -0.5); //Point 7
	this->edge->addPoint(-0.5,  0.5, -0.5); //Point 8

	std::vector<unsigned int> face1(4);
	std::vector<unsigned int> face2(4);
	std::vector<unsigned int> face3(4);
	std::vector<unsigned int> face4(4);
	std::vector<unsigned int> face5(4);
	std::vector<unsigned int> face6(4);
	face1[0] = 1; face1[1] = 2; face1[2] = 3; face1[3] = 4;
	face2[0] = 1; face2[1] = 4; face2[2] = 8; face2[3] = 5;
	face3[0] = 8; face3[1] = 7; face3[2] = 6; face3[3] = 5;
	face4[0] = 6; face4[1] = 7; face4[2] = 3; face4[3] = 2;
	face5[0] = 1; face5[1] = 5; face5[2] = 6; face5[3] = 2;
	face6[0] = 8; face6[1] = 4; face6[2] = 3; face6[3] = 7;

	this->edge->addFace( face1 );
	this->edge->addFace( face2 );
	this->edge->addFace( face3 );
	this->edge->addFace( face4 );
	this->edge->addFace( face5 );
	this->edge->addFace( face6 );

	this->edge->setColor(1.0, 1.0, 1.0);
}

void Grid::_createGrid(){
	if( this->grid != nullptr )
		delete this->grid;
	this->grid = new Mesh();

	int points = 0;
	float boxDistance = 1.0/this->boxNumber;

	for(int i=1 ; i<this->boxNumber ; ++i){
		this->grid->addPoint( 0.5,  0.5, 0.5 - boxDistance*i ); //Point 1 + points
		this->grid->addPoint(-0.5,  0.5, 0.5 - boxDistance*i ); //Point 2 + points
		this->grid->addPoint(-0.5, -0.5, 0.5 - boxDistance*i ); //Point 3 + points
		this->grid->addPoint( 0.5, -0.5, 0.5 - boxDistance*i ); //Point 4 + points

		std::vector<unsigned int> faceA(4);
		faceA[0] = 1 + points;
		faceA[1] = 2 + points;
		faceA[2] = 3 + points;
		faceA[3] = 4 + points;
		this->grid->addFace( faceA );

		points+=4;

		this->grid->addPoint( 0.5, 0.5 - boxDistance*i,  0.5 ); //Point 1 + points
		this->grid->addPoint(-0.5, 0.5 - boxDistance*i,  0.5 ); //Point 2 + points
		this->grid->addPoint(-0.5, 0.5 - boxDistance*i, -0.5 ); //Point 3 + points
		this->grid->addPoint( 0.5, 0.5 - boxDistance*i, -0.5 ); //Point 4 + points

		std::vector<unsigned int> faceB(4);
		faceB[0] = 1 + points;
		faceB[1] = 2 + points;
		faceB[2] = 3 + points;
		faceB[3] = 4 + points;
		this->grid->addFace( faceB );

		points+=4;

		this->grid->addPoint( 0.5 - boxDistance*i,  0.5,  0.5 ); //Point 1 + points
		this->grid->addPoint( 0.5 - boxDistance*i, -0.5,  0.5 ); //Point 2 + points
		this->grid->addPoint( 0.5 - boxDistance*i, -0.5, -0.5 ); //Point 3 + points
		this->grid->addPoint( 0.5 - boxDistance*i,  0.5, -0.5 ); //Point 4 + points

		std::vector<unsigned int> faceC(4);
		faceC[0] = 1 + points;
		faceC[1] = 2 + points;
		faceC[2] = 3 + points;
		faceC[3] = 4 + points;
		this->grid->addFace( faceC );

		points+=4;
	}

	this->grid->setColor(1.0, 1.0, 1.0);
}

void Grid::draw(){
	this->drawInCorner(this->corner);
}

void Grid::drawEdge(){
	this->drawEdgeInCorner(this->corner);
}

void Grid::drawGrid(){
	this->drawGridInCorner(this->corner);
}

void Grid::drawInCorner(const float& x, const float& y, const float& z){
	this->drawInCorner(Vector(x, y, z));
}

void Grid::drawEdgeInCorner(const float& x, const float& y, const float& z){
	this->drawEdgeInCorner(Vector(x, y, z));
}

void Grid::drawGridInCorner(const float& x, const float& y, const float& z){
	this->drawGridInCorner(Vector(x, y, z));
}

void Grid::drawInCorner(const Vector& corner){
	float halfSide = ((float)this->boxNumber)/2.0;

	glPushMatrix();

	glTranslatef(halfSide + this->corner[0], halfSide + this->corner[1], halfSide + this->corner[2]);
	glScalef(this->boxNumber, this->boxNumber, this->boxNumber);
	this->edge->drawInOpenGLContext(GL_LINE_LOOP);
	this->grid->drawInOpenGLContext(GL_LINE_LOOP);

	glPopMatrix();
}

void Grid::drawEdgeInCorner(const Vector& corner){
	float halfSide = ((float)this->boxNumber)/2.0;

	glPushMatrix();

	glTranslatef(halfSide + this->corner[0], halfSide + this->corner[1], halfSide + this->corner[2]);
	glScalef(this->boxNumber, this->boxNumber, this->boxNumber);
	this->edge->drawInOpenGLContext(GL_LINE_LOOP);

	glPopMatrix();
}

void Grid::drawGridInCorner(const Vector& corner){
	float halfSide = ((float)this->boxNumber)/2.0;

	glPushMatrix();

	glTranslatef(halfSide + this->corner[0], halfSide + this->corner[1], halfSide + this->corner[2]);
	glScalef(this->boxNumber, this->boxNumber, this->boxNumber);
	this->grid->drawInOpenGLContext(GL_LINE_LOOP);

	glPopMatrix();

}

void Grid::setEdgeColor(const float& red, const float& green, const float& blue){
	this->edge->setColor(red, green, blue);
}

void Grid::setGridColor(const float& red, const float& green, const float& blue){
	this->grid->setColor(red, green, blue);
}

void Grid::setCorner(const Vector& corner){
	this->corner = corner;
}

void Grid::setCorner(const float& x, const float& y, const float& z){
	this->corner = Vector(x, y, z);
}

void Grid::setBoxNumber(const int& boxNumber){
	this->boxNumber = boxNumber;
	this->_createGrid();
}

Vector Grid::getCorner(){
	return this->corner;
}

int Grid::getBoxNumber(){
	return this->boxNumber;
}

Vector Grid::getStartingPos(){
	return Vector(0.5, 0.5, 0.5);
}

Vector Grid::getStartingForward(){
	return Vector(0, 0, -1);
}

Vector Grid::getStartingUp(){
	return Vector(0, 1, 0);
}

void Grid::updatePosition(Vector& position, Vector& forward, Vector& up){
	position[0] = fmod(position[0] + this->boxNumber, this->boxNumber);
	position[1] = fmod(position[1] + this->boxNumber, this->boxNumber);
	position[2] = fmod(position[2] + this->boxNumber, this->boxNumber);
}

#endif //__GRID__HXX__
