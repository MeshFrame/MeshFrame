/*!
*      \file arcball.h
*      \brief Arcball Interface
*	   \author David Gu
*      \date Documented on 10/08/2010
*
*/

#ifndef _ARCBALL_H_
#define _ARCBALL_H_

#include "../geometry/Point.h"
#include "../geometry/Point2.h"
#include "../geometry/quat.h"

namespace MeshLib
{
/*!
 *	\brief CArcball Interface
 *
 *  Arcball interface is widely used for user interaction,
 *  because it is intuitive and efficient. 
 *  Imagine there is a glass ball centered at the screen
 *  user can hold one point on the ball, and drag the ball
 *  to rotate it. Everything inisde the ball with be roated
 *  accordingly. 
 */
class CArcball
{
public:
	/*!
	 *	CArcball default constructor
	 */
  CArcball(){};

	/*!
	 *	CArcball constructor
	 *  \param width  width of the window
	 *  \param height height of the window
	 *  \param ox     original x position
	 *  \param oy     original y position
	 */
  CArcball( int win_width, int win_height, int ox, int oy )
  {
	  m_radius = (win_width < win_height )? win_width/2: win_height/2;

	  m_center = CPoint2( win_width/2, win_height/2 );

	  CPoint2 p(ox,oy);

	  _plane2sphere( p, m_position );
  };

  /*!
   *	Update the arcball
   *	\param nx current x-position of mouse
   *    \param ny current y-position of mouse
   *    \return quoternion of the roation from the old position to the new position
   */
  CQrot update( int nx, int ny )
 {
    CPoint position;
    _plane2sphere( CPoint2(nx,ny), position );
    CPoint cp = m_position^position;
    CQrot r(m_position * position, cp[0],cp[1],cp[2]);
    m_position = position;

    return r;
 };


private:
	/*!
	 *	mapping a planar point v to the unit sphere point r
	 *  \param v input planar point
	 *  \param r output point on the unit sphere
	 */
  void _plane2sphere( const CPoint2 & v, CPoint & r )
{

  CPoint2 f = v;
  f /= m_radius;

  double l = sqrt( f*f );

  if( l > 1.0 ){
      r=CPoint( f[0]/l, f[1]/l,0);
      return;
  }

  double fz = sqrt( 1 - l * l );

  r = CPoint( f[0],f[1],fz );
};

	/*!
	 *	current position on the unit sphere
	 */
  CPoint   m_position;
  /*!
   *	radius of the sphere
   */
  double   m_radius;
  /*!
   *	center of sphere on the plane
   */
  CPoint2 m_center;
};

};
#endif