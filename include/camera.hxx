#ifndef __CAMERA__HXX__
#define __CAMERA__HXX__

#include "camera.h"
#include "quaternion.h"
#include "vector.h"
#include <cmath>
#include <GL/glu.h>

// -------------------------------------------------------------------------
Camera::
Camera( )
{
  this->setFOV( 45 );
  this->setWindow( 1, 1 );
  this->setPlanes( 1e-2, 10000 );

  this->m_Position = Vector( );
  this->m_Basic = Vector(0, 0, -1);
  this->m_Focus = Quaternion(Vector(0, 0, 0), 0);
  this->m_UpVector = Quaternion(Vector(1, 0, 0), 90);
  this->m_UpVector.normalize();

  this->m_Animating = false;
}

// -------------------------------------------------------------------------
Camera::
~Camera( )
{
}

// -------------------------------------------------------------------------
void Camera::
move( const Vector& dir )
{
  this->m_Position += dir;
}

// -------------------------------------------------------------------------
void Camera::
lookAt( Vector pnt )
{
    pnt-=this->m_Position;
    pnt.normalize();
    this->m_Focus = Quaternion(this->m_Basic*pnt, acos( this->m_Basic / pnt )/_PI_180);
    this->m_Focus.normalize();
}

// -------------------------------------------------------------------------
void Camera::
rotV( Vector forward, const float& angle, const float& camLimit)
{
    Vector prevFocus = this->m_Focus.rotatePoint(this->m_Basic);
    Vector axis = prevFocus*this->m_UpVector.rotatePoint(this->m_Basic);
    axis.normalize();
    Quaternion newR(axis, angle);
    newR.normalize();

    this->m_Focus = newR*this->m_Focus;
    this->m_Focus.normalize();

    if(this->m_Animating == false){
        Vector newFocus = this->m_Focus.rotatePoint(this->m_Basic);
        float newAngle = acos(newFocus/forward)/_PI_180;
        if(newAngle > camLimit){
            Vector newAxis = newFocus*forward;
            newR = Quaternion(newAxis, newAngle-camLimit);
            newR.normalize();
            this->m_Focus = newR*this->m_Focus;
            this->m_Focus.normalize();
        }
    }
}

// -------------------------------------------------------------------------
void Camera::
rotH( Vector forward, const float& angle, const float& camLimit)
{
    Vector axis = this->m_UpVector.rotatePoint(this->m_Basic);
    axis.normalize();
    Quaternion newR(axis, angle);
    newR.normalize();

    this->m_Focus = newR*this->m_Focus;
    this->m_Focus.normalize();

    if(this->m_Animating == false){
        Vector newFocus = this->m_Focus.rotatePoint(this->m_Basic);
        float newAngle = acos(newFocus/forward)/_PI_180;
        if(newAngle > camLimit){
            Vector newAxis = newFocus*forward;
            newR = Quaternion(newAxis, newAngle-camLimit);
            newR.normalize();
            this->m_Focus = newR*this->m_Focus;
            this->m_Focus.normalize();
        }
    }
}

// -------------------------------------------------------------------------
void Camera::
rotate( const Vector& axis, const float& angle )
{
    Quaternion newR(axis, angle);
    newR.normalize();

    this->m_Focus = newR*this->m_Focus;
    this->m_Focus.normalize();
    this->m_UpVector = newR*this->m_UpVector;
    this->m_UpVector.normalize();
}

// -------------------------------------------------------------------------
void Camera::
setPosition( const Vector& position )
{
    this->m_Position = position;
}

// -------------------------------------------------------------------------
void Camera::
setFocus( const Vector& focus )
{
    this->m_Focus = Quaternion(this->m_Basic*focus, acos( this->m_Basic / focus )/_PI_180);
    this->m_Focus.normalize();
}

// -------------------------------------------------------------------------
void Camera::
setUpVector( const Vector& up )
{
    this->m_UpVector = Quaternion(this->m_Basic*up, acos( this->m_Basic / up )/_PI_180);
    this->m_UpVector.normalize();
}

// -------------------------------------------------------------------------
void Camera::
loadCameraMatrix( )
{
    if(this->m_Animating){
        double dt =
        std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now( ) - this->m_InitAnimation
        ).count( );
        dt /= 1000; //from mili to sec
        float newAngle = std::min(90.0, 90.0*(dt/this->m_TurnTime));
        this->rotate(this->m_AnimationAxis, newAngle - this->m_AnimationAngle);

        this->m_AnimationAngle = newAngle;
        if(this->m_AnimationAngle == 90)
            this->m_Animating = false;
    }

    Vector direction = this->m_Focus.rotatePoint(this->m_Basic);
    Vector f = this->m_Position + direction;
    Vector u = this->m_UpVector.rotatePoint(this->m_Basic);

    gluLookAt(
        this->m_Position[ 0 ], this->m_Position[ 1 ], this->m_Position[ 2 ],
        f[ 0 ], f[ 1 ], f[ 2 ],
        u[ 0 ], u[ 1 ], u[ 2 ]
        );
}

// -------------------------------------------------------------------------
void Camera::
setFOV( float a )
{
  this->m_FOV = std::fmod( a, 360 );
}

// -------------------------------------------------------------------------
void Camera::
setPlanes( float n, float f )
{
  this->m_Near = n;
  this->m_Far = f;
}

// -------------------------------------------------------------------------
void Camera::
setWindow( int w, int h )
{
  this->m_WindowWidth = ( w > 0 )? w: 1;
  this->m_WindowHeight = ( h > 0 )? h: 1;
}

// -------------------------------------------------------------------------
void Camera::
loadProjectionMatrix( )
{
  glViewport( 0, 0, this->m_WindowWidth, this->m_WindowHeight );
  gluPerspective(
    this->m_FOV / (float( this->m_WindowWidth ) / float( this->m_WindowHeight )),
    float( this->m_WindowWidth ) / float( this->m_WindowHeight ),
    this->m_Near,
    this->m_Far
    );
}

// -------------------------------------------------------------------------
void Camera::
turn( Vector axis, const float& turnTime)
{
    if(this->m_Animating)
        return;
    this->m_InitAnimation = std::chrono::high_resolution_clock::now();
    this->m_AnimationAngle = 0;
    this->m_AnimationAxis = axis;
    this->m_TurnTime = turnTime;
    this->m_Animating = true;
}

// -------------------------------------------------------------------------
bool Camera::
isTurning( )
{
    return this->m_Animating;
}

// -------------------------------------------------------------------------
void Camera::
setAnimating( const bool& animating)
{
    this->m_Animating = animating;
}

#endif //__CAMERA__HXX__
// eof - camera.hxx
