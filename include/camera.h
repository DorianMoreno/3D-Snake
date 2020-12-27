#ifndef __CAMERA__H__
#define __CAMERA__H__

#include <chrono>
#include "vector.h"
#include "quaternion.h"

/**
 */
class Camera
{
public:
    //! Memory management
    //@{
    Camera( );
    virtual ~Camera( );
    //@}

    void move( const Vector& dir );
    void lookAt( Vector pnt );
    void rotV( Vector forward, const float& angle, const float& camLimit = 50 );
    void rotH( Vector forward, const float& angle, const float& camLimit = 50);
    void rotate( const Vector& axis, const float& angle );

    void setPosition( const Vector& position);
    void setFocus( const Vector& forward);
    void setUpVector( const Vector& up);

    void loadCameraMatrix( );

    void setFOV( float a );
    void setPlanes( float n, float f );
    void setWindow( int w, int h );
    void loadProjectionMatrix( );

    void turn( Vector axis, const float& turnTime );
    bool isTurning();
    void setAnimating(const bool& animating);

//protected:
    float m_FOV;
    float m_Near;
    float m_Far;
    int m_WindowWidth;
    int m_WindowHeight;

    Vector m_Position;
    Vector m_RightVector;
    Vector m_Basic;
    Quaternion m_Focus;
    Quaternion m_UpVector;

    //Needed for animation
    std::chrono::time_point<std::chrono::high_resolution_clock> m_InitAnimation; //time when animation started
    Vector m_AnimationAxis;
    float m_AnimationAngle;
    float m_TurnTime;
    bool m_Animating;
};

#include "camera.hxx"

#endif // __CAMERA__H__

// eof - camera.h
