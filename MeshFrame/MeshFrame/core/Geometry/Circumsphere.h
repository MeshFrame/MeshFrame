#pragma once
#include <Eigen\Dense>
#include <MeshLib\core\TetMesh\TMeshLibHeaders.h>
#include <MeshLib\core\Geometry\Point.h>
#include <vector>

#define SQUARE(X) ( (X)*(X) )

namespace MeshLib
{
	namespace TMeshLib
	{
		class CTetCircumSphere  
		{
		public:
			CTetCircumSphere(CTet *pT);
			CTetCircumSphere(CPoint v0, CPoint v1, CPoint v2, CPoint v3) {
				m_v0 = v0;
				m_v1 = v1;
				m_v2 = v2;
				m_v3 = v3;
				calculateCircumsphere();
			};
			CTetCircumSphere(std::vector<CPoint> & vertice) { 
				m_v0 = vertice[0];
				m_v1 = vertice[1];
				m_v2 = vertice[2];
				m_v3 = vertice[3];
				calculateCircumsphere(); 
			};
			CPoint getCenter() { return m_center; };
			double getRaduis() { return m_radius; };

		private:
			void calculateCircumsphere();
			CPoint m_v0, m_v1, m_v2, m_v3;
			CTet * m_pT;
			CPoint m_center;
			double m_radius;
		};
		CTetCircumSphere::CTetCircumSphere(CTet * pT){

		}
		void CTetCircumSphere::calculateCircumsphere(){
			 Eigen::Matrix4d M_D_x, M_D_y, M_D_z, M_a, M_c;
			 M_D_x << SQUARE(m_v0.norm()) , m_v0[1] , m_v0[2] , 1.0	,
				      SQUARE(m_v1.norm()) , m_v1[1] , m_v1[2] , 1.0	,
				      SQUARE(m_v2.norm()) , m_v2[1] , m_v2[2] , 1.0	,
				      SQUARE(m_v3.norm()) , m_v3[1] , m_v3[2] , 1.0;

			 M_D_y << SQUARE(m_v0.norm()) , m_v0[0] , m_v0[2] , 1.0	,
				      SQUARE(m_v1.norm()) , m_v1[0] , m_v1[2] , 1.0	,
				      SQUARE(m_v2.norm()) , m_v2[0] , m_v2[2] , 1.0	,
				      SQUARE(m_v3.norm()) , m_v3[0] , m_v3[2] , 1.0;
										
			 M_D_z << SQUARE(m_v0.norm()) , m_v0[0] , m_v0[1] , 1.0 ,
				      SQUARE(m_v1.norm()) , m_v1[0] , m_v1[1] , 1.0	,
				      SQUARE(m_v2.norm()) , m_v2[0] , m_v2[1] , 1.0	,
				      SQUARE(m_v3.norm()) , m_v3[0] , m_v3[1] , 1.0;
			 M_a << m_v0[0] , m_v0[1] , m_v0[2] , 1.0 ,
				    m_v1[0] , m_v1[1] , m_v1[2] , 1.0	,
				    m_v2[0] , m_v2[1] , m_v2[2] , 1.0	,
				    m_v3[0] , m_v3[1] , m_v3[2] , 1.0;
			 M_c << SQUARE(m_v0.norm()) , m_v0[0] , m_v0[1] , m_v0[2] ,
				    SQUARE(m_v1.norm()) , m_v1[0] , m_v1[1] , m_v1[2] ,
				    SQUARE(m_v2.norm()) , m_v2[0] , m_v2[1] , m_v2[2] ,
				    SQUARE(m_v3.norm()) , m_v3[0] , m_v3[1] , m_v3[2];

			 double D_x = M_D_x.determinant();
			 double D_y = -M_D_y.determinant();
			 double D_z = M_D_z.determinant();
			 double a = M_a.determinant();
			 double c = M_c.determinant();

			 if (a == 0.0) {
				 m_center = { 0, 0, 0 };
				 m_radius = 0.0;
				 return;
			 }
			 m_center[0] = D_x / (2 * a);
			 m_center[1] = D_y / (2 * a);
			 m_center[2] = D_z / (2 * a);

			 m_radius = sqrt(SQUARE(D_x) + SQUARE(D_y) + SQUARE(D_z) - 4 * a*c) / (2 * abs(a));
		 }
	}
}