#ifndef __MESH__HXX__
#define __MESH__HXX__

#include "mesh.h"
#include <cstring>
#include <deque>
#include <fstream>
#include <list>
#include <sstream>

// -------------------------------------------------------------------------
Mesh::
Mesh( )
{
  this->m_Color[ 0 ] = this->m_Color[ 1 ] = this->m_Color[ 2 ] = 1;
}

// -------------------------------------------------------------------------
Mesh::
Mesh( const std::string& fname )
{
  this->m_Color[ 0 ] = this->m_Color[ 1 ] = this->m_Color[ 2 ] = 1;

  // Open file and put its contents into a string buffer
  std::ifstream in( fname.c_str( ) );
  if( !in )
  {
    in.close( );
    throw std::runtime_error(
      std::string( "Error: Could not open \"" ) + fname + "\""
      );
  } // end if
  std::istringstream buffer(
    std::string(
      ( std::istreambuf_iterator< char >( in ) ),
      std::istreambuf_iterator< char >( )
      )
    );
  in.close( );

  // Read file
  buffer >> this;
}

// -------------------------------------------------------------------------
Mesh::
~Mesh( )
{
}

// -------------------------------------------------------------------------
unsigned long Mesh::
getNumberOfPoints( ) const
{
  return( this->m_Tuple.size( ) / 3 );
}

// -------------------------------------------------------------------------
float* Mesh::
getPoint( unsigned long i )
{
  return( this->m_Tuple.data( ) + ( i * 3 ) );
}

// -------------------------------------------------------------------------
const float* Mesh::
getPoint( unsigned long i ) const
{
  return( this->m_Tuple.data( ) + ( i * 3 ) );
}

// -------------------------------------------------------------------------
void Mesh::
getPoint( unsigned long i, float* p ) const
{
  std::memcpy( p, this->getPoint( i ), 3 * sizeof( float ) );
}

// -------------------------------------------------------------------------
void Mesh::
getPoint( unsigned long i, double* p ) const
{
  const float* fp = this->getPoint( i );
  p[ 0 ] = double( fp[ 0 ] );
  p[ 1 ] = double( fp[ 1 ] );
  p[ 2 ] = double( fp[ 2 ] );
}
//_-------------------------------------------------------------------------
unsigned long Mesh::
getNumberOfNormals( ) const
{
  return( this->m_Normal.size( ) / 3 );
}

// -------------------------------------------------------------------------
float* Mesh::
getNormal( unsigned long i )
{
  return( this->m_Normal.data( ) + ( i * 3 ) );
}

// -------------------------------------------------------------------------
const float* Mesh::
getNormal( unsigned long i ) const
{
  return( this->m_Normal.data( ) + ( i * 3 ) );
}

// -------------------------------------------------------------------------
unsigned long Mesh::
addPoint( float x, float y, float z )
{
  this->m_Tuple.push_back( x );
  this->m_Tuple.push_back( y );
  this->m_Tuple.push_back( z );
  return( this->getNumberOfPoints( ) - 1 );
}

// -------------------------------------------------------------------------
unsigned long Mesh::
addPoint( double x, double y, double z )
{
  this->m_Tuple.push_back( float( x ) );
  this->m_Tuple.push_back( float( y ) );
  this->m_Tuple.push_back( float( z ) );
  return( this->getNumberOfPoints( ) - 1 );
}

// -------------------------------------------------------------------------
unsigned long Mesh::
addPoint( float* p )
{
  return( this->addPoint( p[ 0 ], p[ 1 ], p[ 2 ] ) );
}

// -------------------------------------------------------------------------
unsigned long Mesh::
addPoint( double* p )
{
  return( this->addPoint( p[ 0 ], p[ 1 ], p[ 2 ] ) );
}

// -------------------------------------------------------------------------

unsigned long Mesh::
addNormal( float x, float y, float z )
{
  this->m_Normal.push_back( x );
  this->m_Normal.push_back( y );
  this->m_Normal.push_back( z );
  return( this->getNumberOfNormals( ) - 1 );
}

// -------------------------------------------------------------------------
unsigned long Mesh::
addNormal( double x, double y, double z )
{
  this->m_Normal.push_back( float( x ) );
  this->m_Normal.push_back( float( y ) );
  this->m_Normal.push_back( float( z ) );
  return( this->getNumberOfNormals( ) - 1 );
}


//--------------------------------------------------------------------------
unsigned long Mesh::
addNormal( float* p )
{
  return( this->addNormal( p[ 0 ], p[ 1 ], p[ 2 ] ) );
}

// -------------------------------------------------------------------------
unsigned long Mesh::
addNormal( double* p )
{
  return( this->addNormal( p[ 0 ], p[ 1 ], p[ 2 ] ) );
}

//--------------------------------------------------------------------------
void Mesh::
setPoint( unsigned long i, float x, float y, float z )
{
  float* fp = this->getPoint( i );
  fp[ 0 ] = x;
  fp[ 1 ] = y;
  fp[ 2 ] = z;
}

// -------------------------------------------------------------------------
void Mesh::
setPoint( unsigned long i, double x, double y, double z )
{
  float* fp = this->getPoint( i );
  fp[ 0 ] = float( x );
  fp[ 1 ] = float( y );
  fp[ 2 ] = float( z );
}

// -------------------------------------------------------------------------
void Mesh::
setPoint( unsigned long i, float* p )
{
  std::memcpy( this->getPoint( i ), p, 3 * sizeof( float ) );
}

// -------------------------------------------------------------------------
void Mesh::
setPoint( unsigned long i, double* p )
{
  float* fp = this->getPoint( i );
  fp[ 0 ] = float( p[ 0 ] );
  fp[ 1 ] = float( p[ 1 ] );
  fp[ 2 ] = float( p[ 2 ] );
}

// -------------------------------------------------------------------------
#define Mesh_addFace_IMPL( _c_, _t_ )                                   \
  template<>                                                            \
  void Mesh::                                                           \
  addFace< std::_c_< unsigned _t_ > >(                                  \
    const std::_c_< unsigned _t_ >& f                                   \
    )                                                                   \
  {                                                                     \
    this->m_Topology.push_back( f.size( ) );                            \
    for( const unsigned _t_& v: f )                                     \
      this->m_Topology.push_back( ( unsigned long )( v ) );             \
  }
Mesh_addFace_IMPL( deque, char );
Mesh_addFace_IMPL( deque, short );
Mesh_addFace_IMPL( deque, int );
Mesh_addFace_IMPL( deque, long );
Mesh_addFace_IMPL( deque, long long );

Mesh_addFace_IMPL( list, char );
Mesh_addFace_IMPL( list, short );
Mesh_addFace_IMPL( list, int );
Mesh_addFace_IMPL( list, long );
Mesh_addFace_IMPL( list, long long );

Mesh_addFace_IMPL( vector, char );
Mesh_addFace_IMPL( vector, short );
Mesh_addFace_IMPL( vector, int );
Mesh_addFace_IMPL( vector, long );
Mesh_addFace_IMPL( vector, long long );

// -------------------------------------------------------------------------
#define Mesh_addFaceNormal_IMPL( _c_, _t_ )                                   \
  template<>                                                            \
  void Mesh::                                                           \
  addFaceNormal< std::_c_< unsigned _t_ > >(                                  \
    const std::_c_< unsigned _t_ >& n                                   \
    )                                                                   \
  {                                                                     \
    this->m_TopologyNormal.push_back( n.size( ) );                            \
    for( const unsigned _t_& v: n )                                     \
      this->m_TopologyNormal.push_back( ( unsigned long )( v ) );             \
  }
Mesh_addFaceNormal_IMPL( deque, char );
Mesh_addFaceNormal_IMPL( deque, short );
Mesh_addFaceNormal_IMPL( deque, int );
Mesh_addFaceNormal_IMPL( deque, long );
Mesh_addFaceNormal_IMPL( deque, long long );

Mesh_addFaceNormal_IMPL( list, char );
Mesh_addFaceNormal_IMPL( list, short );
Mesh_addFaceNormal_IMPL( list, int );
Mesh_addFaceNormal_IMPL( list, long );
Mesh_addFaceNormal_IMPL( list, long long );

Mesh_addFaceNormal_IMPL( vector, char );
Mesh_addFaceNormal_IMPL( vector, short );
Mesh_addFaceNormal_IMPL( vector, int );
Mesh_addFaceNormal_IMPL( vector, long );
Mesh_addFaceNormal_IMPL( vector, long long );

// -------------------------------------------------------------------------
float* Mesh::
getColor( )
{
  return( this->m_Color );
}

// -------------------------------------------------------------------------
const float* Mesh::
getColor( ) const
{
  return( this->m_Color );
}

// -------------------------------------------------------------------------
void Mesh::
getColor( float* p ) const
{
  std::memcpy( p, this->m_Color, 3 * sizeof( float ) );
}

// -------------------------------------------------------------------------
void Mesh::
drawInOpenGLContext( GLenum mode )
{
    glColor3fv( this->m_Color );

    unsigned long i = 0;
    while( i < this->m_Topology.size( ) )
    {
        glBegin( mode );
        {
            for( unsigned long j = 0; j < this->m_Topology[i] ; ++j ){
                if(this->m_TopologyNormal.size() >i)
                    glNormal3fv( this->getNormal( this->m_TopologyNormal[ i + j + 1 ] - 1 ) );
                glVertex3fv( this->getPoint( this->m_Topology[ i + j + 1] - 1 ) );
            }
        }
        glEnd( );
        i += this->m_Topology[i]+1;
    } // end while
}

// -------------------------------------------------------------------------
void Mesh::
_strIn( std::istream& in )
{
    this->m_Tuple.clear( );
    this->m_Normal.clear( );
    this->m_Texture.clear( );
    this->m_Topology.clear( );

    std::string line;
    std::getline( in, line );
    while( !in.eof( ) )
    {
        // Get command type
        std::istringstream str( line );
        std::string cmd;
        str >> cmd;

        // Process command
        if( cmd == "v" || cmd == "V" )
        {
            std::vector< float > p;
            while( !str.eof( ) )
            {
                float v;
                str >> v;
                p.push_back( v );
            } // end while
            this->addPoint( p.data( ) );
        }
        else if( cmd == "vn" || cmd == "VN" ){
            std::vector< float > p;
            while( !str.eof( ) )
            {
                float v;
                str >> v;
                p.push_back( v );
            } // end while
            this->addNormal( p.data( ) );
        }
        else if( cmd == "f" || cmd == "F" )
        {
            std::vector< unsigned long > f, t, n;
            while( !str.eof( ) )
            {
                std::string parameters;
                str >> parameters;
                std::istringstream par(parameters);

                std::string v, vt, vn;

                std::getline(par, v, '/');
                std::getline(par, vt, '/');
                std::getline(par, vn, '/');

                f.push_back(stoi(v));
                if(vt.size() > 0);
                //t.push_back(stoi(vt))
                if(vn.size() > 0);
                n.push_back(stoi(vn));

            } // end while
            this->addFace( f );
            this->addFaceNormal( n );
            //pending      this->addTexture( t );
        } // end if

        // Try next line
        std::getline( in, line );
    } // end while
}

// -------------------------------------------------------------------------
void Mesh::
_strOut( std::ostream& out ) const
{
  // Stream geometry
  unsigned long i = 0;
  for( i = 0; i < this->m_Tuple.size( ); ++i )
  {
    if( i % 3 == 0 )
      out << std::endl << "v ";
    out << this->m_Tuple[ i ];
  } // end for

  // Stream normals
  i = 0;
  for( i = 0; i < this->m_Normal.size( ); ++i )
  {
    if( i % 3 == 0 )
      out << std::endl << "v ";
    out << this->m_Tuple[ i ];
  } // end for

  // Stream topology

  i = 0;
  while( i < this->m_Topology.size( ) )
  {
    out << std::endl << "f ";
    for( unsigned long j = 0; j < 3; ++j ){
      out << this->m_Topology[ i + j ] << "/";
      out << this->m_TopologyNormal[ i + j ] << "/";
      out << this->m_Texture[ i + j ] << " ";
      }
    i += 3;
  } // end while
}

#endif // __MESH__HXX__

// eof - Mesh.cxx
