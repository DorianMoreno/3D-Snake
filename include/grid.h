#ifndef __GRID__H__
#define __GRID__H__

#include "vector.h"
#include "mesh.h"
#include <vector>
#include <GL/glut.h>

class Grid
{
public:
	//! Memory management
	//@{
	Grid();
	Grid(const int& boxNumber = 10, const Vector& corner = Vector(0, 0, 0));
	Grid(const int& boxNumber, const float& x, const float& y, const float& z);
	virtual ~Grid();
	//@}

	void draw();
	void drawInCorner(const Vector& corner);
	void drawInCorner(const float& x, const float& y, const float& z);

	void drawEdge();
	void drawEdgeInCorner(const Vector& corner);
	void drawEdgeInCorner(const float& x, const float& y, const float& z);

	void drawGrid();
	void drawGridInCorner(const Vector& corner);
	void drawGridInCorner(const float& x, const float& y, const float& z);

	void setEdgeColor(const float& red, const float& green, const float& blue);
	void setGridColor(const float& red, const float& green, const float& blue);
	void setCorner(const Vector& corner);
	void setCorner(const float& x, const float& y, const float& z);
	void setBoxNumber(const int& boxNumber);

	Vector getCorner();
	int getBoxNumber();

	Vector getStartingPos();
	Vector getStartingForward();
	Vector getStartingUp();

	void updatePosition(Vector& position, Vector& forward, Vector& up);

private:
	int boxNumber;
	Vector corner;

	Mesh* edge;
	Mesh* grid;

	void _createEdge();
	void _createGrid();
};

#include "grid.hxx"

#endif // __GRID__H__
