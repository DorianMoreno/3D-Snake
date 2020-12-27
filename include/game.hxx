#ifndef __GAME__HXX__
#define __GAME__HXX__

#include "game.h"

#define cameraNear 1e-1
#define cameraMult 100
#define turnRollBack 0.7    // 0.0 (rollback is nonexistent) -> 1.0 (goes back to the begining of the block)
#define angleLimit 80
#define nextBox 0.3         // Postion in which you are considered on the next box, 0 <= nextBox < 0.5

#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

Game::
Game( const int& gridSize, const float& initialSpeed, const float& speedIncrease, const float& FOV):
    m_Snake(nullptr),
    m_Camera(nullptr),
    m_Grid(nullptr),
    m_ExternalGrid(nullptr),
    m_Food(nullptr),
    m_GameStarted(true),
    m_Pause(false)
{
    this->m_InitialSpeed = initialSpeed;
    this->m_GridSize = gridSize;
    this->m_FOV = FOV;


    //Create world and snake
    this->m_Score = 0;
    this->m_Grid = new Grid(gridSize);
    this->m_Snake = new Snake(initialSpeed,
        speedIncrease,
        this->m_Grid->getStartingPos(),
        this->m_Grid->getStartingForward(),
        this->m_Grid->getStartingUp()
    );

    this->m_Food = new FoodBlock(this->m_GridSize);
    this->generateFood();
    this->generateTrash();

    //Create and set up camera
    this->m_Camera = new Camera();
    this->m_Camera->setFOV(this->m_FOV);
    this->m_Camera->setPlanes( cameraNear, cameraMult*this->m_GridSize );
    this->m_Camera->setPosition( this->m_Snake->getPosition() );
    this->m_Camera->setFocus( this->m_Snake->getForward() );
    this->m_Camera->setUpVector( this->m_Snake->getUp() );

    //Pre-draw grid
    this->m_List = glGenLists(3);       //+0 = grid, +1 = head, +2 = clone the world
    glNewList(this->m_List, GL_COMPILE);
	{
		this->m_Grid->draw();
	}
	glEndList();

    //Create head block
    SnakeBlock* head = new SnakeBlock();
    glNewList(this->m_List+1, GL_COMPILE);
    {
        head->draw();
    }
    glEndList();



    //Light0 aka Ambient
    GLfloat lightPos[] = {0.0, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    GLfloat ambientLight[] = {0.2, 0.2, 0.2, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    delete head;

    //togglePause();

}

Game::
~Game( )
{
    if(this->m_Snake != nullptr){
        delete (this->m_Snake);
        this->m_Snake = nullptr;
    }
    if(this->m_Camera != nullptr){
        delete (this->m_Camera);
        this->m_Camera = nullptr;
    }
    if(this->m_Grid != nullptr){
        delete (this->m_Grid);
        this->m_Grid = nullptr;
    }
    if(this->m_Food != nullptr){
        delete (this->m_Food);
        this->m_Food = nullptr;
    }
}
//---------------------------------------------------------------------------------------------------
void Game::
resetGame(  )
{
    this->m_Blocks.clear();
    this->m_Obstacles.clear();

    this->m_Snake->reset(this->m_InitialSpeed,
        this->m_Grid->getStartingPos(),
        this->m_Grid->getStartingForward(),
        this->m_Grid->getStartingUp()
        );
    this->generateTrash();
    this->m_Camera->setPosition( this->m_Snake->getPosition() );
    this->m_Camera->setFocus( this->m_Snake->getForward() );
    this->m_Camera->setUpVector( this->m_Snake->getUp() );
    this->m_Camera->setAnimating( false );

    this->m_Score = 0;
    this->m_Pause = false;
    togglePause();
}

void Game::
staticCameraRotation( const float& dx, const float& dy, const float& sensibH, const float& sensibV )
{
    if(dx != 0)
        this->m_Camera->rotH( this->m_Snake->getForward(), -dx*sensibH/50.0, angleLimit);
    if(dy != 0)
        this->m_Camera->rotV( this->m_Snake->getForward(), -dy*sensibV/50.0, angleLimit);
}

void Game::
resizeCamera(const int& w, const int& h)
{
    this->m_Camera->setWindow( w, h );
    this->m_Camera->loadProjectionMatrix( );
}
//---------------------------------------------------------------------------------------------------
void Game::
keyPress( const char& key )
{
    if (m_Pause && key != 'p' && key != 'P'&& key != 'r'&& key != 'R') return;
    switch(key){
	case 'w': case 'W':
        if(!this->m_Camera->isTurning())
		      this->m_Camera->turn(this->m_Snake->turn('u', turnRollBack, nextBox), turnRollBack/this->m_Snake->getSpeed());
		break;
	case 's': case 'S':
        if(!this->m_Camera->isTurning())
		      this->m_Camera->turn(this->m_Snake->turn('d', turnRollBack, nextBox), turnRollBack/this->m_Snake->getSpeed());
		break;
	case 'a': case 'A':
        if(!this->m_Camera->isTurning())
		      this->m_Camera->turn(this->m_Snake->turn('l', turnRollBack, nextBox), turnRollBack/this->m_Snake->getSpeed());
		break;
	case 'd': case 'D':
        if(!this->m_Camera->isTurning())
		      this->m_Camera->turn(this->m_Snake->turn('r', turnRollBack, nextBox), turnRollBack/this->m_Snake->getSpeed());
		break;
	case 'r': case 'R':
        this->resetGame();
		break;
	case 'p': case 'P':
        this->togglePause();
		break;
    }
}

//---------------------------------------------------------------------------------------------------
void Game::
specialKeyPress( const int& key )
{
	switch(key){
        case GLUT_KEY_UP:
            if(!this->m_Camera->isTurning())
                this->m_Camera->turn(this->m_Snake->turn('u', turnRollBack, nextBox), turnRollBack/this->m_Snake->getSpeed());
            break;
        case GLUT_KEY_DOWN:
            if(!this->m_Camera->isTurning())
                this->m_Camera->turn(this->m_Snake->turn('d', turnRollBack, nextBox), turnRollBack/this->m_Snake->getSpeed());
            break;
		case GLUT_KEY_LEFT:
            if(!this->m_Camera->isTurning())
			         this->m_Camera->turn(this->m_Snake->turn('l', turnRollBack, nextBox), turnRollBack/this->m_Snake->getSpeed());
			break;
		case GLUT_KEY_RIGHT:
            if(!this->m_Camera->isTurning())
			         this->m_Camera->turn(this->m_Snake->turn('r', turnRollBack, nextBox), turnRollBack/this->m_Snake->getSpeed());
			break;
	}
}

//---------------------------------------------------------------------------------------------------
void Game::
draw( )
{

    //light params
    //L1 params
    //GLfloat light1Pos[] = {1.0, 1.0, 0.0, 0.5};

    // Camera matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    this->m_Camera->loadProjectionMatrix();

    glMatrixMode(GL_MODELVIEW);
    this->m_Camera->loadCameraMatrix( );
    //Previous positions
    Vector newPos 		= this->m_Snake->currentPos();
    Vector newForward	= this->m_Snake->getForward();
    Vector newUp 		= this->m_Snake->getUp();

    //Draw grid
    glPushMatrix();
        //glDisable(GL_LIGHTING);
        glCallList(this->m_List);
        //glEnable(GL_LIGHTING);
    glPopMatrix();


    glNewList(this->m_List+1, GL_COMPILE);
    {
        this->m_Snake->drawHead();
    }
    glEndList();

    glNewList(this->m_List+2, GL_COMPILE_AND_EXECUTE);
    {

        // Update and draw snake position based on grid rules

        glPushMatrix();
            this->m_Grid->updatePosition(newPos, newForward, newUp);
            this->m_Snake->update(newPos, newForward, newUp, this->m_Blocks);
            this->m_Snake->drawBody();
            if(this->m_Snake->getHeadPos() == this->m_Food->getPosition()){
                this->m_Score += 100;
                this->generateFood();
                this->m_Snake->increaseSize();
            }
            else if(this->collision(this->m_Snake->getHeadPos()) == SNAKE){
                this->m_Snake->setSpeed(0);
                m_HighScore =  m_HighScore > m_Score ? m_HighScore : m_Score;
                togglePause();
            }
            else if(this->collision(this->m_Snake->getHeadPos()) == OBSTACLE){
                this->m_Snake->setSpeed(0);
                m_HighScore =  m_HighScore > m_Score ? m_HighScore : m_Score;
                togglePause();
            }

            //draw food
            this->m_Food->draw();
            //draw obstacles
            for ( auto m: this->m_Obstacles ){
                m->draw();
            }
        glPopMatrix();
    }
	glEndList();

    // this->draw6Directions();
    // this->draw18Directions();
    // this->draw26Directions();
    // this->draw32Directions();
    this->draw56Directions();

    this->m_Camera->setPosition(newPos);

}

//---------------------------------------------------------------------------------------------------
void Game::
draw6Directions(  )
{
    int x[6] = {1, 0, 0, -1,  0,  0};
    int y[6] = {0, 1, 0,  0, -1,  0};
    int z[6] = {0, 0, 1,  0,  0, -1};

    for(int i=0 ; i< 6 ; ++i){
        glPushMatrix();
            glTranslatef(x[i]*this->m_GridSize, y[i]*this->m_GridSize, z[i]*this->m_GridSize);
            glCallList(this->m_List+2);
            glCallList(this->m_List+1);
        glPopMatrix();
    }
}

//---------------------------------------------------------------------------------------------------
void Game::
draw18Directions(  )
{
    int x[12] = {1, 1, 0, -1, -1,  0,  1,  1,  0, -1, -1,  0};
    int y[12] = {1, 0, 1, -1,  0, -1, -1,  0,  1,  1,  0, -1};
    int z[12] = {0, 1, 1,  0, -1, -1,  0, -1, -1,  0,  1,  1};

    this->draw6Directions();
    for(int i=0 ; i< 12 ; ++i){
        glPushMatrix();
            glTranslatef(x[i]*this->m_GridSize, y[i]*this->m_GridSize, z[i]*this->m_GridSize);
            glCallList(this->m_List+2);
            glCallList(this->m_List+1);
        glPopMatrix();
    }
}

//---------------------------------------------------------------------------------------------------
void Game::
draw26Directions(  )
{
    int x[8] = {1, -1,  1,  1, -1,  1, -1, -1};
    int y[8] = {1, -1,  1, -1,  1, -1,  1, -1};
    int z[8] = {1, -1, -1,  1,  1, -1, -1,  1};

    this->draw18Directions();
    for(int i=0 ; i< 8 ; ++i){
        glPushMatrix();
            glTranslatef(x[i]*this->m_GridSize, y[i]*this->m_GridSize, z[i]*this->m_GridSize);
            glCallList(this->m_List+2);
            glCallList(this->m_List+1);
        glPopMatrix();
    }
}

//---------------------------------------------------------------------------------------------------
void Game::
draw32Directions(  )
{
    int x[6] = {2, 0, 0, -2,  0,  0};
    int y[6] = {0, 2, 0,  0, -2,  0};
    int z[6] = {0, 0, 2,  0,  0, -2};

    this->draw26Directions();
    for(int i=0 ; i< 6 ; ++i){
        glPushMatrix();
            glTranslatef(x[i]*this->m_GridSize, y[i]*this->m_GridSize, z[i]*this->m_GridSize);
            glCallList(this->m_List+2);
            glCallList(this->m_List+1);
        glPopMatrix();
    }
}

//---------------------------------------------------------------------------------------------------
void Game::
draw56Directions(  )
{
    this->draw32Directions();
    int x[24] = {2, 2, 0, -2, -2,  0,  2,  2,  0, -2, -2,  0, 1, 1, 0, -1, -1,  0,  1,  1,  0, -1, -1,  0};
    int y[24] = {1, 0, 2, -1,  0, -2, -1,  0,  2,  1,  0, -2, 2, 0, 1, -2,  0, -1, -2,  0,  1,  2,  0, -1};
    int z[24] = {0, 1, 1,  0, -1, -1,  0, -1, -1,  0,  1,  1, 0, 2, 2,  0, -2, -2,  0, -2, -2,  0,  2,  2};

    for(int i=0 ; i< 24 ; ++i){
        glPushMatrix();
            glTranslatef(x[i]*this->m_GridSize, y[i]*this->m_GridSize, z[i]*this->m_GridSize);
            glCallList(this->m_List+2);
            glCallList(this->m_List+1);
        glPopMatrix();
    }
}

//---------------------------------------------------------------------------------------------------
void Game::
generateFood( )
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0, 1.0);

    Vector pos((int)(dist(mt)*this->m_GridSize), (int)(dist(mt)*this->m_GridSize), (int)(dist(mt)*this->m_GridSize));
    while(this->m_Blocks.find(pos) != this->m_Blocks.end()){
        pos = Vector((int)(dist(mt)*this->m_GridSize), (int)(dist(mt)*this->m_GridSize), (int)(dist(mt)*this->m_GridSize));
    }

    this->m_Food->setPosition(pos);
}

//---------------------------------------------------------------------------------------------------

void Game::
generateTrash(){
    m_Obstacles.clear();
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0, 1.0);

    for (int i = 0; i < m_GridSize*m_GridSize*0.01; ++i){

        Vector pos( (int)(dist(mt)*this->m_GridSize),
                    (int)(dist(mt)*this->m_GridSize),
                    (int)(dist(mt)*this->m_GridSize));

        while(this->m_Blocks.find(pos) != this->m_Blocks.end()){
            pos = Vector(   (int)(dist(mt)*this->m_GridSize),
                            (int)(dist(mt)*this->m_GridSize),
                            (int)(dist(mt)*this->m_GridSize));
        }
            FoodBlock* fb =  new FoodBlock(m_GridSize, pos);
            fb->setColorSolid( Vector( 0.2 , 0.9 , 0.9  ) );
            m_Obstacles.push_back(fb);
            m_Blocks[pos] = OBSTACLE;

    }

}

//---------------------------------------------------------------------------------------------------
int Game::
collision(const Vector& pos)
{
    if(this->m_Blocks.find(pos) == this->m_Blocks.end())
        return -1;
    else
        return this->m_Blocks[pos];
}

int Game::
getHScore(){
    return m_HighScore;
}

void Game::
setHScore(int hS){
    this->m_HighScore = hS;
}

int Game::
getScore(){
    return m_Score;
}

void Game::
togglePause(){
    static float oldSpeed;


    m_Pause = !m_Pause;
    m_GameStarted = !m_GameStarted;

    oldSpeed = m_Pause ? m_Snake->getSpeed(): oldSpeed;
    m_Snake->setSpeed( m_Pause ? 0 : oldSpeed );


}

bool Game::
paused(){
 return m_Pause;
}

void Game::
pauseScreen(){
//oopsie
int w = glutGet(GLUT_WINDOW_WIDTH),
    h = glutGet(GLUT_WINDOW_HEIGHT);

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0,w,h,0);

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

glRasterPos2i( 15, h/8 );

/*
______________________________________
|\t~[ICG Project]~
|>code-name:\tSnake-3D
|
|>Controls
|\twasd\tor\t↑←↓→
|\tup down left right
|
|\tp: toggle pause
|\tr: restart game
|
|>-Authors-
|\tAlejandro Arciniegas
|\tDorian Moreno
∟--------------------------------------

______________________________________\n|\t~[ICG Project]~\n|>code-name:\tSnake-3D:infinity\n|\n|>Controls\n|\twasd\tor\t↑←↓→\n|\tup down left right\n|\n|\tp: toggle pause\n|\tr: restart game\n|\n|>-Authors-\n|\tAlejandro Arciniegas\n|\tDorian Moreno\n∟--------------------------------------\n
*/

//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
//glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
glColor3f(1.0f, 0.0f, 1.0f);
std::string text = "______________________________________\n|\t~[ICG Project]~\n|>code-name:\tSnake-3D : infinity\n|\n|>Controls\n|\twasd\tor\tarrow keys\n|\t(movement)\n|\n|\tp: start/pause\n|\tr: reiniciar\n|\tpink -> grow : other -> die\n|\n|\n|>-Authors-\n|\tAlejandro Arciniegas\n|\tDorian Moreno\n---------------------------------------\n";
const unsigned char* t = reinterpret_cast<const unsigned char *>( text.c_str() );
glutBitmapString(GLUT_BITMAP_HELVETICA_18,t);


}

#endif // __GAME__HXX__

// eof - game.hxx
