#ifndef __GAME__H__
#define __GAME__H__

#include <chrono>
#include <GL/freeglut.h>

#include <GL/glu.h>
#include <GL/glut.h>
#include <map>
#include <random>
#include <vector>

#include "vector.h"
#include "camera.h"
#include "snake.h"
#include "grid.h"
#include "block.h"

#define SNAKE 0
#define OBSTACLE 1

class Game
{
public:
  //! Memory management
  //@{
  Game(  const int& gridSize = 10, const float& initialSpeed = 2.5f, const float& speedIncrease = 0.3333f, const float& FOV = 110);
  virtual ~Game( );
  //@}

  void resetGame(  );
  void staticCameraRotation( const float& dx, const float& dy, const float& sensibH = 1.0, const float& sensibV = 1.0 );
  void resizeCamera(const int& w, const int& h);

  void keyPress(const char& key);
  void specialKeyPress(const int& key);

  void draw( );
  void pauseScreen();
  void draw6Directions(  );
  void draw18Directions(  );
  void draw26Directions(  );
  void draw32Directions(  );
  void draw56Directions(  );

  void generateFood(  );
  void generateTrash( );

  int collision(const Vector& position);

  int getHScore();
  void setHScore(int hS= 0);
  int getScore();
  void togglePause();
  bool paused();

protected:
    Snake* m_Snake;
    Camera* m_Camera;
    Grid* m_Grid;
    Grid* m_ExternalGrid;

    int m_Score;
    int m_HighScore;
    bool m_GameStarted;
    bool m_Pause;

    GLuint m_List;

    FoodBlock* m_Food;
    std::vector<FoodBlock*> m_Obstacles;
    std::map<Vector, int> m_Blocks;

    float m_InitialSpeed;
    float m_GridSize;
    float m_FOV;


};

#include "game.hxx"

#endif // __GAME__H__

// eof - game.h
