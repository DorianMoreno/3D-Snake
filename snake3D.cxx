/* -------------------------------------------------------------------------
	|Compilation on linux:

	g++ -std=c++17 snake3D.cxx -lm -lGL -lGLU -lglut -o snake3D

	Execution on linux (after compilation):

	./snake3D

	To move snake, use keys: W, A, S and D
   -------------------------------------------------------------------------
 */
#include "game.h"
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

// -------------------------------------------------------------------------
Game* myGame;

std::chrono::time_point< std::chrono::high_resolution_clock > lastTime; //last fps report
ulong frames;
bool erzte = true;

//--------------------------------------------------------------------------

//inits and destroyers
void initWorld( int argc, char* argv[] );
void destroyWorld();

// -------------------------------------------------------------------------
int fpsHelper();
void displayCbk( );
void idleCbk( );
void resizeCbk( int w, int h );
void keyboardCbk( unsigned char key, int x, int y );
void specialKeyboardCbk( int key, int x, int y);
void mouseClickCbk( int button, int state, int x, int y );
void mouseMoveCbk( int x, int y );

// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
    //Init OpenGL context
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( 1000 , 1000 );
	glutCreateWindow( "Snake 3D - Cubic universe" );

    // Init world
    try{
		initWorld(argc, argv);

        glutDisplayFunc( displayCbk );
        glutIdleFunc( idleCbk );
		glutReshapeFunc( resizeCbk );

        glutSpecialFunc( specialKeyboardCbk );
        glutKeyboardFunc( keyboardCbk);
        glutMouseFunc( mouseClickCbk );
        glutPassiveMotionFunc( mouseMoveCbk );
		glutSetCursor( GLUT_CURSOR_NONE );

		//myGame->togglePause();

		glutMainLoop(  );

        destroyWorld();
        return( 0 );
    }
    catch( std::exception& err ){
      std::cerr << err.what( ) << std::endl;
      return( 1 );
    } // end if

}
// -------------------------------------------------------------------------
void initWorld( int argc, char* argv[] ){

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDepthRange(0.1f, 1.0f);

	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glShadeModel(GL_FLAT);
	glEnable(GL_COLOR_MATERIAL);
	glutFullScreen();

	//glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE ) ;


	// OpenGL initialization
	glClearColor( 0, 0, 0, 0 );

	myGame = new Game();

    //Prepare for calculating frames
    frames = 0;
    lastTime = std::chrono::high_resolution_clock::now();

}
// -------------------------------------------------------------------------
void destroyWorld()
{
	if( myGame != nullptr )
		delete myGame;
}

//--------------------------------------------------------------------------
void keyboardCbk( unsigned char key, int x, int y )
{
    switch( key )
    {
	case 27: // ESC
		std::exit( 0 );
		break;
	default:
		myGame->keyPress(key);
		break;
    } // end switch
}
// -------------------------------------------------------------------------
void specialKeyboardCbk( int key, int x, int y)
{
	myGame->specialKeyPress(key);
}

// -------------------------------------------------------------------------
void resizeCbk( int w, int h )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	myGame->resizeCamera(w, h);
}
// -------------------------------------------------------------------------
void fpsHelper(int &fps){
    double s =
    std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::high_resolution_clock::now( ) - lastTime
        ).count( );
    s/= 1000;

    if (s>3){
        fps = frames/s;
        frames = 0;
        lastTime = std::chrono::high_resolution_clock::now( );
    }
}
// -------------------------------------------------------------------------
void idleCbk( )
{
	glutPostRedisplay( );
}

// -------------------------------------------------------------------------
void mouseClickCbk( int button, int state, int x, int y )
{

}

// -------------------------------------------------------------------------
void mouseMoveCbk( int x, int y )
{
	if (!myGame->paused()) {
		float height = glutGet(GLUT_WINDOW_HEIGHT);
		float width = glutGet(GLUT_WINDOW_WIDTH);
		float dx = (float)(x - width/2);
		float dy = (float)(y - height/2);

		myGame->staticCameraRotation(dx, dy, 0.5, 0.5);

		glutPostRedisplay( );
	}
}


void drawData(int w, int h, int fps){
//setting data box
	//glViewport(0,0,10,10);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(0,w,h, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i( 10, 20 );
    std::string text = SSTR ( "fps: "<< fps << "\tScore: " << myGame->getScore() << "\tHighscore: " << myGame->getHScore());
    const unsigned char* t = reinterpret_cast<const unsigned char *>( text.c_str() );
	glColor3f(1.0f, 1.0f, 1.0f);

    glutBitmapString(GLUT_BITMAP_HELVETICA_18,t);


}

// -------------------------------------------------------------------------
void displayCbk( )
{
	int const w = glutGet(GLUT_WINDOW_WIDTH);
    int const h = glutGet(GLUT_WINDOW_HEIGHT);


	if (!myGame->paused())
		glutWarpPointer(
			w / 2,
			h / 2 );

	frames++;
	static int fps =0;
	fpsHelper(fps);


	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//single viewport, might make it double (and double the trouble)
    glViewport(0, 0, w, h);

	// Redraw
	if(myGame->paused())
		myGame->pauseScreen();
	else
		myGame->draw();

	//data window
	drawData(w,h,fps);
	if(erzte) {myGame->togglePause(); erzte =false;}
	glutSwapBuffers( );
}
