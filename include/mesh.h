#ifndef __MESH__H__
#define __MESH__H__

#include <iostream>
#include <vector>
#include <GL/gl.h>

/**
 */
class Mesh
{
public:
  //! Streaming methods
  //@{
  friend std::istream& operator>>( std::istream& i, Mesh& s )
    {
      s._strIn( i );
      return( i );
    }

  friend std::istream& operator>>( std::istream& i, Mesh* s )
    {
      s->_strIn( i );
      return( i );
    }

  friend std::ostream& operator<<( std::ostream& o, const Mesh& s )
    {
      s._strOut( o );
      return( o );
    }

  friend std::ostream& operator<<( std::ostream& o, const Mesh* s )
    {
      s->_strOut( o );
      return( o );
    }
  //@}

public:
  //! Memory management
  //@{
  Mesh( );
  Mesh( const std::string& fname );
  virtual ~Mesh( );
  //@}

  //! Geometry-related methods
  //@{
  unsigned long getNumberOfPoints( ) const;
  float* getPoint( unsigned long i );
  const float* getPoint( unsigned long i ) const;
  void getPoint( unsigned long i, float* p ) const;
  void getPoint( unsigned long i, double* p ) const;

  unsigned long addPoint( float x, float y, float z );
  unsigned long addPoint( double x, double y, double z );
  unsigned long addPoint( float* p );
  unsigned long addPoint( double* p );

  unsigned long getNumberOfNormals( ) const;
  float* getNormal( unsigned long i );
  const float* getNormal( unsigned long i ) const;


  unsigned long addNormal( float x, float y, float z );
  unsigned long addNormal( double x, double y, double z );
  unsigned long addNormal( float* p );
  unsigned long addNormal( double* p );


  void setPoint( unsigned long i, float x, float y, float z );
  void setPoint( unsigned long i, double x, double y, double z );
  void setPoint( unsigned long i, float* p );
  void setPoint( unsigned long i, double* p );
  //@}

  //! Topology-related methods
  //@{
  template< class _TContainer >
  void addFace( const _TContainer& f );

  template< class _TContainer >
  void addFaceNormal( const _TContainer& n );

  template< class _TContainer >
  void addTexture( const _TContainer& t );

  template< class _TIterator >
  inline void addFace( _TIterator b, _TIterator e )
    {
      std::vector< unsigned long > f;
      for( _TIterator i = b; i != e; ++i )
        f.push_back( ( unsigned long )( *i ) );
      this->addFace( f );
    }

  //! vector+natural+variadic arguments
  template< class _TNatural, class... _TArgs >
  inline void addFace(
    std::vector< unsigned long >& f,
    const _TNatural& x,
    _TArgs... args
    )
    {
      f.push_back( ( unsigned long )( x ) );
      this->addFace( f, args... );
    }

  //! natural+variadic arguments
  template< class _TNatural, class... _TArgs >
  inline void addFace( const _TNatural& x, _TArgs... args )
    {
      std::vector< unsigned long > f;
      f.push_back( ( unsigned long )( x ) );
      this->addFace( f, args... );
    }
  //@}

  //! Colors
  //@{
  float* getColor( );
  const float* getColor( ) const;
  void getColor( float* p ) const;

  template< class _TRed, class _TGreen, class _TBlue >
  inline void setColor( _TRed r, _TGreen g, _TBlue b )
    {
      this->m_Color[ 0 ] = float( r );
      this->m_Color[ 1 ] = float( g );
      this->m_Color[ 2 ] = float( b );
    }
  //@}

  //! All the magic happens here
  virtual void drawInOpenGLContext( GLenum mode );

protected:
  //! Here's where the real streaming is done
  //@{
  void _strIn( std::istream& in );
  void _strOut( std::ostream& out ) const;
  //@}

protected:
  std::vector< float >          m_Tuple; //(n,n+1,n+2)
  std::vector< float >          m_Normal;
  std::vector< unsigned long >  m_Topology; //(v1,v2,..,vn)
  std::vector< unsigned long >  m_TopologyNormal; //(v1,v2,..,vn)
  std::vector< unsigned long >  m_Texture;
  float                         m_Color[ 3 ]; //r,g,b
};


#include "mesh.hxx"

#endif // __MESH__H__

// eof - Mesh.h
