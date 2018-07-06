/*!
*      \file Point.h
*      \brief Three dimensional point class
*	   \author David Gu
*      \date 10/07/2010
*
*/

#ifndef _MESHLIB_POINT_H_
#define _MESHLIB_POINT_H_

#include <assert.h>
#include <math.h>
#include <string>
#include <sstream>


namespace MeshLib
{

	/*!
	*	\brief CPointInBuffer class, three dimensional point
	*
	*	Three dimensional point or vector \f$(x,y,z)\f$, inner product, cross product, scale multiplication etc.
	*/
	class CPointInBuffer {

	public:
		/*!
		* CPointInBuffer constructor, specifying \f$(x,y,z)\f$
		*/
		CPointInBuffer(double* data, double x, double y, double z) : v(data)
		{
			v[0] = x; v[1] = y; v[2] = z;
		};
		/*!
		*	CPointInBuffer default constructor, initialized as (0,0,0)
		*/
		CPointInBuffer(double* data) : v(data)
		{
			v[0] = v[1] = v[2] = 0;
		};
		/*!
		*	CPointInBuffer destructor
		*/
		~CPointInBuffer() {};

		double * coords() { return v; };

		/*!
		*	reference to  \f$(x,y,z)\f$ value
		*   \param i index
		*   \return CPointInBuffer[i]
		*/
		double & operator[](int i) { assert(0 <= i && i < 3); return v[i]; };
		/*!
		*	constant (x,y,z) value
		*   \param i index
		*   \return CPointInBuffer[i]
		*/
		double   operator()(int i) const { assert(0 <= i && i < 3); return v[i]; };
		/*!
		*	constant  \f$(x,y,z)\f$ value
		*   \param i index
		*   \return CPointInBuffer[i]
		*/

		double   operator[](int i) const { assert(0 <= i && i < 3); return v[i]; };

		/*!
		*	norm of the CPointInBuffer \f$\sqrt{x^2+y^2+z^2}\f$
		*/
		double norm() const { return sqrt(fabs(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])); };

		/*!
		* Add another point to the current point
		* \param p
		* \return cuccrent point is added by p.
		*/
		CPointInBuffer  & operator += (const CPointInBuffer & p) { v[0] += p(0); v[1] += p(1); v[2] += p(2); return *this; };
		/*!
		* Substract another point to the current point
		* \param p
		* \return cuccrent point is substracted by p.
		*/
		CPointInBuffer  & operator -= (const CPointInBuffer & p) { v[0] -= p(0); v[1] -= p(1); v[2] -= p(2); return *this; };
		/*!
		* Multiply by a scalar
		* \param s scalar
		* \return current point is multipolied by s.
		*/

		CPointInBuffer  & operator *= (const double  s) { v[0] *= s; v[1] *= s; v[2] *= s; return *this; };
		/*!
		* Divide by a scalar
		* \param s scalar
		* \return current point is divided by s.
		*/
		CPointInBuffer  & operator /= (const double  s) { v[0] /= s; v[1] /= s; v[2] /= s; return *this; };

		/*!
		* dot product
		* \param p another point
		* \return dot product of current point with p.
		*/

		double   operator*(const CPointInBuffer & p) const
		{
			return v[0] * p[0] + v[1] * p[1] + v[2] * p[2];
		};

		/*!
		* Add another point to the current point
		* \param p
		* \return cuccrent point is added by p.
		*/
		CPointInBuffer   operator+(const CPointInBuffer & p) const
		{
			CPointInBuffer r(v[0] + p[0], v[1] + p[1], v[2] + p[2]);
			return r;
		};
		/*!
		* Substract another point to the current point
		* \param p
		* \return cuccrent point is substracted by p.
		*/
		CPointInBuffer   operator-(const CPointInBuffer & p) const
		{
			CPointInBuffer r(v[0] - p[0], v[1] - p[1], v[2] - p[2]);
			return r;
		};
		/*!
		* Multiply by a scalar
		* \param s scalar
		* \return current point is multipolied by s.
		*/
		CPointInBuffer   operator*(const double s) const
		{
			CPointInBuffer r(v[0] * s, v[1] * s, v[2] * s);
			return r;
		};
		/*!
		* Divide by a scalar
		* \param s scalar
		* \return current point is divided by s.
		*/
		CPointInBuffer   operator/(const double s) const
		{
			CPointInBuffer r(v[0] / s, v[1] / s, v[2] / s);
			return r;
		};

		bool operator==(const CPointInBuffer &p) const
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
		CPointInBuffer operator^(const CPointInBuffer & p2) const
		{
			CPointInBuffer r(v[1] * p2[2] - v[2] * p2[1],
				v[2] * p2[0] - v[0] * p2[2],
				v[0] * p2[1] - v[1] * p2[0]);
			return r;
		};

		/*!
		* negate the point
		* \return the negative of the current point
		*/
		CPointInBuffer operator-() const
		{
			CPointInBuffer p(-v[0], -v[1], -v[2]);
			return p;
		};

		/*! overload stream operator << */
		friend std::ostream & operator << (std::ostream & os, const CPointInBuffer & p)
		{
			os << p[0] << " " << p[1] << " " << p[2];
			return os;
		};

	protected:
		/*!
		* \f$(x,y,z)\f$ value
		*/
		double* v;
	};

	/*!
	*	Read a CPointInBuffer from a string
	* \param str string
	* \param p three dimenionsal point
	* \return the (x,y,z) value is read from str and assigned to p.
	*/
	inline void operator>>(const std::string & str, CPointInBuffer &p)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of("()"));
		t.erase(t.find_last_not_of("()") + 1);
		std::istringstream iss(t);

		iss >> p[0] >> p[1] >> p[2];
	}

	inline CPointInBuffer operator*(const double s, CPointInBuffer p)
	{
		return CPointInBuffer(p[0] * s, p[1] * s, p[2] * s);
	}

}//name space TMeshLib

#endif //_TMESHLIB_POINT_H_ defined