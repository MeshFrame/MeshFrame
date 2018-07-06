#pragma once
#include "../../core/TetMesh/TMeshLibHeaders.h"
#include "../../algorithm/Foliation/BorderType.h"
#include "../../algorithm/Foliation/BaryCoordinates4D.h"
#include "TFaceFL.h"
#include <random>
#define TET_DIRECTION_DEMENSION 4
#define OUTGOING -1.0
#define INCOMING 1.0
#define INF 999999999999999999999.9
namespace MeshLib
{
	namespace TMeshLib
	{
		class CTFaceFL;
		class CTetFL;
		
		class CTetFL : public CTet, CBaryCoordinates4D {
		public :
			CTetFL() : d(0.0, 0.0, 0.0, 0.0), dSign(0.0, 0.0, 0.0, 0.0) {};

			typedef TetHalfFaceIterator<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL> TetHalfFaceIterator;
			bool foliating = false;

			void assignDirectionFL(CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL> *pMesh);
			const CPoint4 & getD() { return d; };
			void markDeleted() { deleted = true; };
			bool hasDeleted() { return deleted; };

			void markSinkFace() { sinkFace = true; };
			bool isSinkFace() { return sinkFace; };

		private:
			//The barycentric constant direction.
			CPoint4 d;
			//The sign of each entry of the constant direction.
			CPoint4 dSign;
			bool deleted = false;
			bool sinkFace = false;
			void assignWithInteriorDirection(double);
			CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL> *pThisMesh;
			void setCorrespondingVertexHalfface(CHalfFace*, int);
		};
		void CTetFL::assignDirectionFL(CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL> *pMesh) {
			pThisMesh = pMesh;
			foliating = true;
			assignWithInteriorDirection(OUTGOING);
			//std::cout << "FACE:" << id() << std::endl;
			foliating = false;
			std::cout << "Direction is:" << d[0] << ',' << d[1] << ',' << d[2] << ',' << d[3] << std::endl;
		}
		void CTetFL::assignWithInteriorDirection(double direction) {
			static std::default_random_engine generator(time(NULL));
			static std::uniform_real_distribution<double> randReal(0.1, 1.0);

			double dSign[TET_DIRECTION_DEMENSION]; 
			TetHalfFaceIterator THfItr(pThisMesh, this);
			bool hasParrallel = false;
			int parrallelIndex = TET_DIRECTION_DEMENSION;
			std::vector<BorderType::BorderType> borderTypes(TET_DIRECTION_DEMENSION);
			std::vector<std::string> borderTypesString(TET_DIRECTION_DEMENSION);
			std::cout << "Edge type:";
			for (int i = 0; i < TET_DIRECTION_DEMENSION; ++i) {
				CHalfFace * pHf = *THfItr;
				CTFaceFL * pF = (CTFaceFL *)pHf->face();
				setCorrespondingVertexHalfface(pHf, i);
				++THfItr;
				BorderType::BorderType faceType = pF->getBorderType();
				borderTypes[i] = faceType;
				borderTypesString[i] = pF->getBorderTypeStr();
				std::cout << "|" << borderTypesString[i];
				switch (faceType)
				{
				case BorderType::source:
					dSign[i] = 1;
					break;
				case BorderType::sink:
					dSign[i] = -1;
					break;
				case BorderType::parrallel:
					dSign[i] = 0;

					if (hasParrallel) {
						//When this happens, a face have more than one parrallel boundaries. 
						std::cerr << "Error:a face have more than one parrallel faces." << std::endl;
						getchar();
						exit(0);
					}
					hasParrallel = true;
					parrallelIndex = i;
					break;
				case BorderType::interior:
					dSign[i] = direction;
					break;
				default:
					break;
				}

			}
			std::cout << std::endl;
			//randomly give a direction under direction constraint.

				//last entry = 0 - all rest entries
			double lastEntry = 0;
			//Make sure the last entry has correct sign , and all the entries of d sum to 0.
			do {
				lastEntry = 0;
				for (int i = 0; i < TET_DIRECTION_DEMENSION; ++i) {
					d[i] = 0;
				}
				for (int i = 0; i < TET_DIRECTION_DEMENSION - 1; ++i) {
					d[i] = dSign[i] * randReal(generator);
					lastEntry -= d[i];
				}
			} while (lastEntry * dSign[TET_DIRECTION_DEMENSION - 1] < 0);
			d[TET_DIRECTION_DEMENSION - 1] = lastEntry;
			d /= d.norm();
			//getchar();
		}
		void CTetFL::setCorrespondingVertexHalfface(CHalfFace* pHf, int i) {
			setVpHalfface(pHf, i);
			int V0id = pHf->key(0), V1id = pHf->key(1), V2id = pHf->key(2);

			for (int k = 0; k < TET_DIRECTION_DEMENSION; ++k) {
				if (V0id !=this->vertex(k)->id() 
					&& V1id != this->vertex(k)->id()
					&& V2id != this->vertex(k)->id()) 
				{
					setVpVertice(this->vertex(k), i);
					return;
				}
			}
			setVpVertice(NULL, i);
			return;
		}

	}
}