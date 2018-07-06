/*!
*      \file Point.h
*      \brief Three dimensional point class
*	   \author David Gu
*      \date 10/07/2010
*
*/

#ifndef _MESHLIB_POINT_F_H_
#define _MESHLIB_POINT_F_H_

#include <assert.h>
#include <math.h>
#include <string>
#include <sstream>
//#include <MeshLib\3rdParty\Eigen\Dense>


namespace MeshLib
{

	/*!
	*	\brief CPointF class, three dimensional point
	*
	*	Three dimensional point or vector \f$(x,y,z)\f$, inner product, cross product, scale multiplication etc.
	*/
	class CPointF{

	public:
		/*!
		 * CPointF constructor, specifying \f$(x,y,z)\f$
		 */
		CPointF(float x, float y, float z){ v[0] = x; v[1] = y; v[2] = z; };
		/*!
		*	CPointF default constructor, initialized as (0,0,0)
		*/
		CPointF() { v[0] = v[1] = v[2] = 0; };
		/*!
		*	CPointF constructor, from Eigen::Vector3d
		*/
		//CPointF(Eigen::Vector3d vec) { v[0] = vec[0]; v[1] = vec[1]; v[2] = vec[2]; };

		/*!
		*	CPointF destructor
		*/
		~CPointF(){};

		/*!
		*	the pointer of the coordinater
		*   \return the pointer float *
		*/
		float * coord() { return v; };

		/*!
		*	reference to  \f$(x,y,z)\f$ value
		*   \param i index
		*   \return CPointF[i]
		*/
		float & operator[](int i)		  { assert(0 <= i && i < 3); return v[i]; };
		/*!
		*	constant (x,y,z) value
		*   \param i index
		*   \return CPointF[i]
		*/
		float   operator()(int i) const { assert(0 <= i && i < 3); return v[i]; };
		/*!
		*	constant  \f$(x,y,z)\f$ value
		*   \param i index
		*   \return CPointF[i]
		*/

		float   operator[](int i) const { assert(0 <= i && i < 3); return v[i]; };

		/*!
		 *	norm of the CPointF \f$\sqrt{x^2+y^2+z^2}\f$
		 */
		float norm() const { return sqrt(fabs(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])); };
		/*!
		*	squared norm of the CPointF \f$\sqrt{x^2+y^2+z^2}\f$
		*/
		float norm2() const { return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); };

		/*!
		 * Add another point to the current point
		 * \param p
		 * \return cuccrent point is added by p.
		 */
		CPointF  & operator += (const CPointF & p) { v[0] += p(0); v[1] += p(1); v[2] += p(2); return *this; };
		/*!
		 * Substract another point to the current point
		 * \param p
		 * \return cuccrent point is substracted by p.
		 */
		CPointF  & operator -= (const CPointF & p)  { v[0] -= p(0); v[1] -= p(1); v[2] -= p(2); return *this; };
		/*!
		 * Multiply by a scalar
		 * \param s scalar
		 * \return current point is multipolied by s.
		 */

		CPointF  & operator *= (const float  s) { v[0] *= s; v[1] *= s; v[2] *= s; return *this; };
		/*!
		 * Divide by a scalar
		 * \param s scalar
		 * \return current point is divided by s.
		 */
		CPointF  & operator /= (const float  s) { v[0] /= s; v[1] /= s; v[2] /= s; return *this; };

		/*!
		 * dot product
		 * \param p another point
		 * \return dot product of current point with p.
		 */

		float   operator*(const CPointF & p) const
		{
			return v[0] * p[0] + v[1] * p[1] + v[2] * p[2];
		};

		/*!
		 * Add another point to the current point
		 * \param p
		 * \return cuccrent point is added by p.
		 */
		CPointF   operator+(const CPointF & p) const
		{
			CPointF r(v[0] + p[0], v[1] + p[1], v[2] + p[2]);
			return r;
		};
		/*!
		 * Substract another point to the current point
		 * \param p
		 * \return cuccrent point is substracted by p.
		 */
		CPointF   operator-(const CPointF & p) const
		{
			CPointF r(v[0] - p[0], v[1] - p[1], v[2] - p[2]);
			return r;
		};
		/*!
		 * Multiply by a scalar
		 * \param s scalar
		 * \return current point is multipolied by s.
		 */
		CPointF   operator*(const float s) const
		{
			CPointF r(v[0] * s, v[1] * s, v[2] * s);
			return r;
		};
		/*!
		 * Divide by a scalar
		 * \param s scalar
		 * \return current point is divided by s.
		 */
		CPointF   operator/(const float s) const
		{
			CPointF r(v[0] / s, v[1] / s, v[2] / s);
			return r;
		};

		bool operator==(const CPointF &p) const
		{
			if (v[0] == p[0] && v[1] == p[1] && v[2] == p[2])
			{
				return true;
			}
			else
			{
				return false;
			}
		};

		/*!
		 * Cross product
		 * \param p2 another point
		 * \return cross product of the current point with p2.
		 * \f[
		 * \left|
		 \begin{array}{ccc}
		 x_1& y_1 & z_1\\
		 x_2 &y_2 & z_2\\
		 \mathbf{e}_1 & \mathbf{e}_2 & \mathbf{e}_3\\
		 \end{array}
		 * \right|
		 * \f]
		 */
		CPointF operator^(const CPointF & p2) const
		{
			CPointF r(v[1] * p2[2] - v[2] * p2[1],
				v[2] * p2[0] - v[0] * p2[2],
				v[0] * p2[1] - v[1] * p2[0]);
			return r;
		};

		/*!
		 * negate the point
		 * \return the negative of the current point
		 */
		CPointF operator-() const
		{
			CPointF p(-v[0], -v[1], -v[2]);
			return p;
		};

		/*! overload stream operator << */
		friend std::ostream & operator << (std::ostream & os, const CPointF & p)
		{
			os << p[0] << " " << p[1] << " " << p[2];
			return os;
		};

		//Eigen::Vector3d getEigenVec() {
		//	return Eigen::Vector3d(v[0], v[1], v[2]);
		//}

	protected:
		/*!
		* \f$(x,y,z)\f$ value
		*/
		float v[3];
	};

	/*!
	 *	Read a CPointF from a string
	 * \param str string
	 * \param p three dimenionsal point
	 * \return the (x,y,z) value is read from str and assigned to p.
	 */
	inline void operator>>(const std::string & str, CPointF &p)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of("()"));
		t.erase(t.find_last_not_of("()") + 1);
		std::istringstream iss(t);

		iss >> p[0] >> p[1] >> p[2];
	}

	inline CPointF operator*(const float s, CPointF p)
	{
		return CPointF(p[0] * s, p[1] * s, p[2] * s);
	}

}//name space TMeshLib

#endif //_TMESHLIB_POINT_H_ defined