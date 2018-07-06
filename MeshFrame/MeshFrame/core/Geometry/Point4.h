#pragma once
namespace MeshLib
{
	namespace TMeshLib {
		/*!
		*	\brief CPoint class, three dimensional point
		*
		*	Three dimensional point or vector \f$(x,y,z)\f$, inner product, cross product, scale multiplication etc.
		*/
		class CPoint4 {

		public:
			/*!
			* CPoint constructor, specifying \f$(x,y,z)\f$
			*/
			CPoint4(double x, double y, double z, double w) { v[0] = x; v[1] = y; v[2] = z; v[3] = w; };
			/*!
			*	CPoint default constructor, initialized as (0,0,0)
			*/
			CPoint4() { v[0] = v[1] = v[2] = v[3] = 0; };
			/*!
			*	CPoint destructor
			*/
			~CPoint4() {};

			double * coords() { return v; };

			/*!
			*	reference to  \f$(x,y,z)\f$ value
			*   \param i index
			*   \return CPoint[i]
			*/
			double & operator[](int i) { assert(0 <= i && i < 4); return v[i]; };
			/*!
			*	constant (x,y,z) value
			*   \param i index
			*   \return CPoint[i]
			*/
			double   operator()(int i) const { assert(0 <= i && i < 4); return v[i]; };
			/*!
			*	constant  \f$(x,y,z)\f$ value
			*   \param i index
			*   \return CPoint[i]
			*/

			double   operator[](int i) const { assert(0 <= i && i < 4); return v[i]; };

			/*!
			*	norm of the CPoint \f$\sqrt{x^2+y^2+z^2}\f$
			*/
			double norm() const { return sqrt(fabs(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3])); };

			/*!
			* Add another point to the current point
			* \param p
			* \return cuccrent point is added by p.
			*/
			CPoint4  & operator += (const CPoint4 & p) { v[0] += p(0); v[1] += p(1); v[2] += p(2); v[3] += p(3); return *this; };
			/*!
			* Substract another point to the current point
			* \param p
			* \return cuccrent point is substracted by p.
			*/
			CPoint4  & operator -= (const CPoint4 & p) { v[0] -= p(0); v[1] -= p(1); v[2] -= p(2); v[3] -= p(3); return *this; };
			/*!
			* Multiply by a scalar
			* \param s scalar
			* \return current point is multipolied by s.
			*/

			CPoint4  & operator *= (const double  s) { v[0] *= s; v[1] *= s; v[2] *= s; v[3] *= s; return *this; };
			/*!
			* Divide by a scalar
			* \param s scalar
			* \return current point is divided by s.
			*/
			CPoint4  & operator /= (const double  s) { v[0] /= s; v[1] /= s; v[2] /= s; v[3] /= s; return *this; };

			/*!
			* dot product
			* \param p another point
			* \return dot product of current point with p.
			*/

			double   operator*(const CPoint4 & p) const
			{
				return v[0] * p[0] + v[1] * p[1] + v[2] * p[2] + v[3] * p[3];
			};

			/*!
			* Add another point to the current point
			* \param p
			* \return cuccrent point is added by p.
			*/
			CPoint4   operator+(const CPoint4 & p) const
			{
				CPoint4 r(v[0] + p[0], v[1] + p[1], v[2] + p[2], v[3] + p[3]);
				return r;
			};
			/*!
			* Substract another point to the current point
			* \param p
			* \return cuccrent point is substracted by p.
			*/
			CPoint4   operator-(const CPoint4 & p) const
			{
				CPoint4 r(v[0] - p[0], v[1] - p[1], v[2] - p[2], v[3] - p[3]);
				return r;
			};
			/*!
			* Multiply by a scalar
			* \param s scalar
			* \return current point is multipolied by s.
			*/
			CPoint4   operator*(const double s) const
			{
				CPoint4 r(v[0] * s, v[1] * s, v[2] * s, v[3] * s);
				return r;
			};
			/*!
			* Divide by a scalar
			* \param s scalar
			* \return current point is divided by s.
			*/
			CPoint4   operator/(const double s) const
			{
				CPoint4 r(v[0] / s, v[1] / s, v[2] / s, v[3] / s);
				return r;
			};

			bool operator==(const CPoint4 &p) const
			{
				if (v[0] == p[0] && v[1] == p[1] && v[2] == p[2] && v[3] == p[3])
				{
					return true;
				}
				else
				{
					return false;
				}
			};


			/*!
			* negate the point
			* \return the negative of the current point
			*/
			CPoint4 operator-() const
			{
				CPoint4 p(-v[0], -v[1], -v[2], -v[3]);
				return p;
			};

			/*! overload stream operator << */
			friend std::ostream & operator << (std::ostream & os, const CPoint4 & p)
			{
				os << p[0] << " " << p[1] << " " << p[2] << " " << p[3];
				return os;
			};

		protected:
			/*!
			* \f$(x,y,z)\f$ value
			*/
			double v[4];
		};

		/*!
		*	Read a CPoint from a string
		* \param str string
		* \param p three dimenionsal point
		* \return the (x,y,z) value is read from str and assigned to p.
		*/
		inline void operator>>(const std::string & str, CPoint4 &p)
		{
			std::string t = str;
			t.erase(0, t.find_first_not_of("()"));
			t.erase(t.find_last_not_of("()") + 1);
			std::istringstream iss(t);

			iss >> p[0] >> p[1] >> p[2] >> p[3];
		}

		inline CPoint4 operator*(const double s, CPoint4 p)
		{
			return CPoint4(p[0] * s, p[1] * s, p[2] * s, p[3] * s);
		}

	}//name space TMeshLib#pragma once
}

