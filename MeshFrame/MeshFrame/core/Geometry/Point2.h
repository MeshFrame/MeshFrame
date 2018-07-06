/*!
*      \file Point2.h
*      \brief Two dimensional point class
*	   \author David Gu
*      \date 10/07/2010
*
*/

#include <math.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>

#ifndef _MESHLIB_POINT2_H_
#define _MESHLIB_POINT2_H_



namespace MeshLib{

/*!
*	\brief CPoint2 class, two dimensional point
*
*	Two dimensional point \f$(x,y)\f$
*/
class CPoint2
{
	public:
	/*!
	*	CPoint2 default constructor, it is initialized to be (0,0)
	*/
    CPoint2(){ m_c[0] = 0; m_c[1] = 0; };
	/*!
	*	CPoint2 class copy operator
	*/
    CPoint2( const CPoint2 & uv ) {	m_c[0] = uv.m_c[0]; m_c[1] = uv.m_c[1];};
	/*!
	 *	CPoint2 class destructor
	 */
    ~CPoint2(){};
	
	 /*!
	  *	CPoint2 constructor with \f$(x,y)\f$
	  */
	 CPoint2( double  x, double y ) { m_c[0] = x; m_c[1] = y; };
	
	 /*!
	  *	reference to the values, CPoint2[0] x value ; CPoint2[1] y value; 
	  */
    double & operator[]( int i ) { 	assert( i < 2 && i >= 0 ); return m_c[i]; };

		bool operator==( const CPoint2 & uv ) 
		{
			return ( m_c[0] == uv.m_c[0] && m_c[1] == uv.m_c[1] );
		}

	 /*!
	  *	constant values, CPoint2[0] x value ; CPoint2[1] y value; 
	  */
    double  operator[]( int i ) const { 	assert( i < 2 && i >= 0 ); return m_c[i]; };

	/*! Divide by a scalor
	 *
	 */
    void operator/=( double s )
    {
      m_c[0]/=s;
      m_c[1]/=s;
    }

	/*! The norm of CPoint2 \f$\sqrt{x^2+y^2}\f$
	 *
	 */
	double norm() { return sqrt( m_c[0] * m_c[0] + m_c[1] * m_c[1] ); };

	/*! The square norm of CPoint2 \f$x^2+y^2\f$
	 *
	 */
	double norm2() { return  m_c[0] * m_c[0] + m_c[1] * m_c[1]; };

	/*!
	 * Add another point to the current point
	 * \param p
	 * \return cuccrent point is added by p.
	 */
	CPoint2  & operator += ( const CPoint2 & p) { m_c[0] += p[0]; m_c[1] += p[1]; return *this; }; 
	/*!
	 * Substract another point to the current point
	 * \param p
	 * \return cuccrent point is substracted by p.
	 */
	CPoint2  & operator -= ( const CPoint2 & p)  { m_c[0] -= p[0]; m_c[1] -= p[1]; return *this; };
	/*!
	 * Multiply by a scalar
	 * \param s scalar
	 * \return current point is multipolied by s.
	 */
	
	CPoint2  & operator *= ( const double  s) { m_c[0] *= s; m_c[1] *= s; return *this; };

	private:
		/*!
		* (x,y) value 
		*/
		double m_c[2];

};

/*!
*	summation of two two dimensional points
*   \param uv0 input two dimensional point
*   \param uv1 input two dimensional point
*   \return summation of uv0 and uv1
*/
inline CPoint2 operator+(  CPoint2 & uv0,  CPoint2 & uv1 )
{
	CPoint2 uv( uv0[0]+uv1[0], uv0[1] + uv1[1] );
	return uv;
};

/*!
*	difference between two two dimensional points
*   \param uv0 input two dimensional point
*   \param uv1 input two dimensional point
*   \return difference between uv0 and uv1
*/
inline CPoint2 operator-(  CPoint2 & uv0,  CPoint2 & uv1 )
{
	CPoint2 uv( uv0[0]-uv1[0], uv0[1] - uv1[1] );
	return uv;
};

/*!
*	multiply by a scalor
*   \param uv0 input two dimensional point
*   \param s  scalor
*   \return product of uv0 and s
*/
inline CPoint2 operator*(  CPoint2 & uv0, const double s )
{
	CPoint2 uv( uv0[0] * s, uv0[1] * s );
	return uv;
};

/*!
*	divide by a scalor
*   \param uv0 input two dimensional point
*   \param s  scalor
*   \return quotient between uv0 and s
*/

inline CPoint2 operator/(  CPoint2 & uv0, const double s )
{
	CPoint2 uv( uv0[0] / s, uv0[1] / s );
	return uv;
};

/*!
*	summation of two two dimensional points
*   \param uv0 input two dimensional point
*   \param uv1 input two dimensional point
*   \return summation of uv0 and uv1
*/
inline CPoint2 operator+(  const CPoint2 & uv0,  const CPoint2 & uv1 )
{
	CPoint2 uv( uv0[0]+uv1[0], uv0[1] + uv1[1] );
	return uv;
};

/*!
*	difference between two two dimensional points
*   \param uv0 input two dimensional point
*   \param uv1 input two dimensional point
*   \return difference between uv0 and uv1
*/
inline CPoint2 operator-(  const CPoint2 & uv0,  const CPoint2 & uv1 )
{
	CPoint2 uv( uv0[0]-uv1[0], uv0[1] - uv1[1] );
	return uv;
};

/*!
*	multiply by a scalor
*   \param uv0 input two dimensional point
*   \param s  scalor
*   \return product of uv0 and s
*/
inline CPoint2 operator*(  const CPoint2 & uv0, const double s )
{
	CPoint2 uv( uv0[0] * s, uv0[1] * s );
	return uv;
};

/*!
*	divide by a scalor
*   \param uv0 input two dimensional point
*   \param s  scalor
*   \return quotient between uv0 and s
*/
inline CPoint2 operator/(  const CPoint2 & uv0, const double s )
{
	CPoint2 uv( uv0[0] / s, uv0[1] / s );
	return uv;
};

/*!
 *	square of the norm of the CPoint2 \f$x^2+y^2\f$
 *  \param uv input two dimensional point
 *  \return square of the norm of the point
 */
inline double mag2(  CPoint2 & uv )
{
	return uv[0] * uv[0] + uv[1] * uv[1];
};

/*!
 *	norm of the CPoint2 \f$ \sqrt{x^2+y^2}\f$
 *  \param uv input two dimensional point
 *  \return norm of the point
 */
inline double mag(  CPoint2 & uv )
{
	return sqrt(uv[0] * uv[0] + uv[1] * uv[1]);
};

/*!
 *	cross product of two CPoint2
 *  \param uv1 the first  two diemenional point
 *  \param uv2 the second two diemenional point
 *  \return the cross product of the two points
 *  \f[
 *  \left|
 *	\begin{array}{cc}
 *  x_1 & y_1\\
 *  x_2 & y_2 \\
 *  \end{array}
 *	\right|
 *  \f]
 */

inline double cross(  CPoint2 uv1,  CPoint2 uv2 )
{
	return (uv1[0] * uv2[1] - uv1[1] * uv2[0]);
};


/*!
 *	inner product of two CPoint2
 *  \param a the first  two diemenional point
 *  \param b the second two diemenional point
 *  \return the inner product of the two points
 */
inline double operator*( CPoint2 a, CPoint2 b )
{
    return a[0] * b[0] + a[1] * b[1];
}

/*!
 *	cross product of two CPoint2
 *  \param uv1 the first  two diemenional point
 *  \param uv2 the second two diemenional point
 *  \return the cross product of the two points
 */

inline double operator^(  CPoint2 uv1,  CPoint2 uv2 )
{
	return (uv1[0] * uv2[1] - uv1[1] * uv2[0]);
};

/*!
 *	read in a CPoint2 from a string
 *  \param str input string
 *  \param c   the reference to a point
 *  \return the value of (x,y) read out from the string is assigned bo c
 */

inline void operator>>(const std::string & str, CPoint2& c )
{
	std::string t = str;
	t.erase(0, t.find_first_not_of("()") );
	t.erase(t.find_last_not_of("()") + 1);
	std::istringstream iss( t );

	iss >> c[0] >> c[1];
}

}; //namespace

#endif
