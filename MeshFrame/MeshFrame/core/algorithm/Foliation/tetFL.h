#pragma once
#include "../../core/TetMesh/TMeshLibHeaders.h"
#include "../../algorithm/Foliation/BorderType.h"
#include "../../algorithm/Foliation/BaryCoordinates4D.h"
#include "TFaceFL.h"
#include <random>
#include <list>
#include <sstream>
#include "DebugSetting.h"

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
			bool inList = false;
			std::list<int> introducingList;
			int introducingSimplexId;

			void assignDirectionFL(CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL> *pMesh);
			bool assignDirectionFLFlag(CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL> *pMesh);
			//bool assignDirectionFLReverseIteratively(CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL> *pMesh);
			void assignDirectionFLReverse(CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL> *pMesh);
			const CPoint4 & getD() { return d; };
			void markDeleted() { deleted = true; };
			void markUndeleted() { deleted = false; };
			bool hasDeleted() { return deleted; };

			void markSinkFace() { sinkFace = true; };
			bool isSinkFace() { return sinkFace; };
			CPoint paraMapping(CPoint4 baryPoint);
			void setCentricParameters(CPoint parameters) { centricParameters = parameters; };

			void _to_string();
		private:
			//The barycentric constant direction.
			CPoint4 d;
			//The sign of each entry of the constant direction.
			CPoint4 dSign;
			bool deleted = false;
			bool sinkFace = false;
			bool assignWithInteriorDirection(double);
			CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL> *pThisMesh;
			void setCorrespondingVertexHalfface(CHalfFace*, int);
			int findMinimumStep(double *a, int n);
			CPoint traceBack(CPoint4 baryPoint, double& backLength, CTFaceFL *&rootEdge);
			double traceForward(CPoint4 baryPoint);
			CPoint bary2Decartes(CPoint4 baryPoint);
			CPoint4 calculateNewBary(CTetFL* pFNext, int touchEdgeIndex, CPoint4 baryPointOnEdge);
			CPoint centricParameters;
		};
		void CTetFL::assignDirectionFL(CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL> *pMesh) {
			pThisMesh = pMesh;
			foliating = true;
			ShowDialog(std::cout << "Tet ID:" << id() << ".";);
			bool unused = assignWithInteriorDirection(OUTGOING);
			//std::cout << "FACE:" << id() << std::endl;
			foliating = false;
			ShowDialog(std::cout << "Direction is:" << d[0] << ',' << d[1] << ',' << d[2] << ',' << d[3] << std::endl;);
		}
		inline bool CTetFL::assignDirectionFLFlag(CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL>* pMesh)
		{
			bool succeed = false;
			pThisMesh = pMesh;
			foliating = true;
			ShowDialog(std::cout << "Tet ID:" << id() << ".";);
			succeed = assignWithInteriorDirection(OUTGOING);
			foliating = false;
			if (succeed) {
				//std::cout << "FACE:" << id() << std::endl;
				ShowDialog(std::cout << "Direction is:" << d[0] << ',' << d[1] << ',' << d[2] << ',' << d[3] << std::endl;);
			}
			else {
				std::cout << "Assign Direction Failed." << std::endl;

			}
			return succeed;
		}
		inline void CTetFL::assignDirectionFLReverse(CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CTFaceFL, CTetFL>* pMesh)
		{
			pThisMesh = pMesh;
			foliating = true;
			ShowDialog(std::cout << "Tet ID:" << id() << ".";);
			assignWithInteriorDirection(INCOMING);
			//std::cout << "FACE:" << id() << std::endl;
			foliating = false;
			ShowDialog(std::cout << "Direction is:" << d[0] << ',' << d[1] << ',' << d[2] << ',' << d[3] << std::endl;);
		}
		inline CPoint CTetFL::paraMapping(CPoint4 baryPoint)
		{
			if (this->isSinkFace())
				return CPoint(0.0, 0.0, 0.0);
			CPoint parameters(0, 0, 0);
			double backLength = 0, forwardLength = 0;
			CPoint4 root(0, 0, 0, 0);
			CTFaceFL* rootFace;
			CPoint baryOnEdge = traceBack(baryPoint, backLength, rootFace);
			forwardLength = traceForward(baryPoint);
			assert(forwardLength + backLength > 0);
			parameters[2] = backLength / (forwardLength + backLength);
			//parameters[0] = rootEdge->paraMapping1D(baryOnEdge);
			//parameters[0] = rootEdge->getAnotherParameter(root);
			return parameters;
			return CPoint();
		}
		void CTetFL::_to_string()
		{
			static std::stringstream _sstream;
			_sstream.str("");
			_sstream.clear();
			_sstream << "parameters=(" << centricParameters[0] << " " << centricParameters[1] << " " << centricParameters[2] << ")";
			m_string = _sstream.str();
		}
		bool CTetFL::assignWithInteriorDirection(double direction) {
			static std::default_random_engine generator(time(NULL));
			static std::uniform_real_distribution<double> randReal(0.1, 1.0);

			double dSign[TET_DIRECTION_DEMENSION]; 
			TetHalfFaceIterator THfItr(pThisMesh, this);
			bool hasParrallel = false;
			int parrallelIndex = TET_DIRECTION_DEMENSION;
			std::vector<BorderType::BorderType> borderTypes(TET_DIRECTION_DEMENSION);
			std::vector<std::string> borderTypesString(TET_DIRECTION_DEMENSION);
			ShowDialog(std::cout << "Edge type:";);
			for (int i = 0; i < TET_DIRECTION_DEMENSION; ++i) {
				CHalfFace * pHf = *THfItr;
				CTFaceFL * pF = (CTFaceFL *)pHf->face();
				setCorrespondingVertexHalfface(pHf, i);
				++THfItr;
				BorderType::BorderType faceType = pF->getBorderType();
				borderTypes[i] = faceType;
				borderTypesString[i] = pF->getBorderTypeStr();
				ShowDialog(std::cout << "|" << borderTypesString[i];);
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
			ShowDialog(std::cout << std::endl;);
			//randomly give a direction under direction constraint.

			//judge if it's a legal direction assignment
			bool hasPositive = false, hasNegative = false;
			for (int i = 0; i < TET_DIRECTION_DEMENSION; i++) {
				if (dSign[i] > 0) {
					hasPositive = true;
				}
				else if(dSign[i] < 0){
					hasNegative = true;
				}
			}
			if (!(hasNegative && hasPositive)) {
				return false;
			}

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
			return true;
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

		inline int CTetFL::findMinimumStep(double * a, int n)
		{
			int t;
			double min = INF;
			for (int i = 0; i<n; i++)
				if (min>a[i] && a[i]>0)
				{
					min = a[i];
					t = i;
				}
			assert(min > 0 && min <  INF);
			return t;
		}

		inline CPoint CTetFL::traceBack(CPoint4 baryPoint, double & backLength, CTFaceFL *& rootEdge)
		{
			CPoint4 dBack = -d;
			double step[TET_DIRECTION_DEMENSION] =
			{
				dBack[0] != 0 ? -baryPoint[0] / dBack[0] : INF,
				dBack[1] != 0 ? -baryPoint[1] / dBack[1] : INF,
				dBack[2] != 0 ? -baryPoint[2] / dBack[2] : INF,
				dBack[3] != 0 ? -baryPoint[3] / dBack[3] : INF
			};
			int touchFaceIndex = findMinimumStep(step, TET_DIRECTION_DEMENSION);
			CPoint4 baryPointOnEdge = baryPoint + step[touchFaceIndex] * dBack;
			assert(-0.0000001 < baryPointOnEdge[touchFaceIndex] && baryPointOnEdge[touchFaceIndex] < 0.0000001);
			CPoint track = bary2Decartes(step[touchFaceIndex] * dBack);
			backLength += track.norm();

			CTFaceFL* pF = (CTFaceFL*)getCorrespondingPHalfface(touchFaceIndex)->face();
			//std::cout << "Touch Face type:" << pF->getOriginalTypeStr() << "," << pF->boundary() << std::endl;
			if (pF->getOriginalType() == BorderType::source) {
				rootEdge = pF;
				//return calculateBaryOnEdge(pF, this, baryPointOnEdge);
				return CPoint(0, 0, 0);
			}
			else {
				assert(getCorrespondingPHalfface(touchFaceIndex)->dual() != NULL);
				CTetFL *pTNext = (CTetFL*)getCorrespondingPHalfface(touchFaceIndex)->dual()->tet();
	
				//calculate the new barycentric coordinates in the next face
				CPoint4 newBray = calculateNewBary(pTNext, touchFaceIndex, baryPointOnEdge);

				return pTNext->traceBack(newBray, backLength, rootEdge);
			}
			return CPoint();
		}

		inline double CTetFL::traceForward(CPoint4 baryPoint)
		{
			double forthLength = 0;
			double step[TET_DIRECTION_DEMENSION] =
			{
				d[0] != 0 ? -baryPoint[0] / d[0] : INF,
				d[1] != 0 ? -baryPoint[1] / d[1] : INF,
				d[2] != 0 ? -baryPoint[2] / d[2] : INF,
				d[3] != 0 ? -baryPoint[3] / d[3] : INF
			};
			int touchFaceIndex = findMinimumStep(step, TET_DIRECTION_DEMENSION);
			CPoint4 baryPointOnEdge = baryPoint + step[touchFaceIndex] * d;
			assert(-0.0000001 < baryPointOnEdge[touchFaceIndex] && baryPointOnEdge[touchFaceIndex] < 0.0000001);
			CPoint track = bary2Decartes(step[touchFaceIndex] * d);
			forthLength += track.norm();

			CTFaceFL* pF = (CTFaceFL*)getCorrespondingPHalfface(touchFaceIndex)->face();

			if (pF->getOriginalType() == BorderType::sink) {
				return forthLength;
			}
			else {
				//find the pointer to next face on the track back
				assert(getCorrespondingPHalfface(touchFaceIndex)->dual() != NULL);
				CTetFL *pTNext = (CTetFL*)getCorrespondingPHalfface(touchFaceIndex)->dual()->tet();
				//calculate the new barycentric coordinates in the next face
				CPoint4 newBray = calculateNewBary(pTNext, touchFaceIndex, baryPointOnEdge);

				return forthLength + pTNext->traceForward(newBray);
			}
			return 1;
		}

		inline CPoint CTetFL::bary2Decartes(CPoint4 baryPoint)
		{
			CPoint p = getCorrespondingPVertex(0)->position() * baryPoint[0]
				+ getCorrespondingPVertex(1)->position() * baryPoint[1]
				+ getCorrespondingPVertex(2)->position() * baryPoint[2]
				+ getCorrespondingPVertex(3)->position() * baryPoint[3];
			return p;
		}

		inline CPoint4 CTetFL::calculateNewBary(CTetFL * pTNext, int touchEdgeIndex, CPoint4 baryPointOnEdge)
		{
			CPoint4 NewBary(0.0, 0.0, 0.0, 0.0);
			for (int i = 0; i < TET_DIRECTION_DEMENSION; ++i) {
				CVertex* pV = pTNext->getCorrespondingPVertex(i);
				for (int j = 0; j < TET_DIRECTION_DEMENSION; ++j) {
					if (j != touchEdgeIndex && pV->id() == this->getCorrespondingPVertex(j)->id()) {
						NewBary[i] = baryPointOnEdge[j];
					}
				}
			}
			return NewBary;
		}

	}
}