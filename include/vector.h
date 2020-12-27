#ifndef __VECTOR__H__
#define __VECTOR__H__

#define _PI 3.1415926535897932384626433832795
#define _PI_180 ( _PI / 180.0 )

/**
 */
class Vector
{
public:
    //! Memory management
    //@{
    Vector( const float& x = 0, const float& y = 0, const float& z = 0 );
    Vector( float* p );
    Vector( double* p );
    Vector( const Vector& r );
    Vector& operator=( const Vector& r );
    virtual ~Vector( );
    //@}

    //! Random acces
    //@{
    float& operator[]( const unsigned int& i );
    const float& operator[]( const unsigned int& i ) const;
    //@}

    //! Base arithmetic
    //@{
    Vector operator+( const Vector& r ) const;
    Vector& operator+=( const Vector& r );

    Vector operator-( const Vector& r ) const;
    Vector& operator-=( const Vector& r );
    //@}

    //! Comparation
    //@{
    bool operator<( const Vector& r ) const;
    bool operator==( const Vector& r ) const;
    //@}

    //! Dot product
    //@{
    float operator/( const Vector& r ) const;
    //@}

    //! Cross product
    //@{
    Vector operator*( const Vector& r ) const;
    Vector& operator*=( const Vector& r );
    //@}

    //! Scalar product
    //@{
    Vector operator*( const float& r ) const;
    Vector operator*( const double& r ) const;

    Vector& operator*=( const float& r );
    Vector& operator*=( const double& r );

    Vector operator/( const float& r ) const;
    Vector operator/( const double& r ) const;

    Vector& operator/=( const float& r );
    Vector& operator/=( const double& r );
    //@}

    //! Normalization
    //@{
    float getNorm( ) const;
    void normalize( );
    //@}

    void print();
protected:
    float m_V[ 3 ];
};

#include "vector.hxx"

#endif // __VECTOR__H__

// eof - Vector.h
