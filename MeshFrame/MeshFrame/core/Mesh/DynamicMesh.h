#ifndef _MESHLIB_DYNAMIC_MESH_H_
#define _MESHLIB_DYNAMIC_MESH_H_ 

#include "BaseMesh.h"
namespace MeshLib {

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    class DynamicMesh:public CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>
    {
    public:
        /*! return the opposite vertex of an edge */
        VertexType * oppVertex(VertexType * pV, EdgeType * pE);

        /*! return the halfedge of the specified edge of a face */
        HalfEdgeType * faceEdgeHalfedge(EdgeType * pE, FaceType * pF);

        /*! return the oppsite vertex of an edge in a face */
        VertexType * oppVertex(EdgeType * pE, FaceType * pF);

        /*! return halfedge of an edge ,and the source vertex of halfedge is pV */
        HalfEdgeType * edgeVertexHalfedge1(EdgeType * pE, VertexType * pV);

        /*! return halfedge of an edge ,and the source vertex of halfedge is pV */
        HalfEdgeType * edgeVertexHalfedge2(EdgeType * pE, VertexType * pV);

        /*! insert halfedge into vertex */
        void enterHalfedge(HalfEdgeType * pHe, VertexType * pV1);

        /*! remove halfedge from vertex */
        void removeHalfedge(HalfEdgeType * pHe, VertexType * pV1);

        /*! destroy face and modified its neighbour info */
        void destoryFace(FaceType * pF);

        /*! collapse an edge to vertex vs */
        void collapseEdgeVertex(EdgeType * pE);

        /*! collapse an edge to vertex vs */
        void collapseEdgeVertexNM(EdgeType * pE);
    };

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    typename VertexType * DynamicMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::oppVertex(VertexType * pV, EdgeType * pE)
    {
        VertexType * pV1 = edgeVertex1(pE);
        VertexType * pV2 = edgeVertex2(pE);
        if (pV == pV1)return pV2;
        if (pV == pV2)return pV1;
    }

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    HalfEdgeType * DynamicMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::faceEdgeHalfedge(EdgeType * pE, FaceType * pF)
    {
        if (edgeFace1(pE) == pF) { return (HalfEdgeType *)pE->halfedge(); }
        if (edgeFace2(pE) == pF) { return (HalfEdgeType *)pE->halfedge()->he_sym(); }
    }

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    VertexType * DynamicMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::oppVertex(EdgeType * pE, FaceType * pF)
    {
        HalfEdgeType * pHe = faceEdgeHalfedge(pE, pF);
        return (VertexType *)pHe->he_next()->vertex();
    }

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    HalfEdgeType * DynamicMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::edgeVertexHalfedge1(EdgeType * pE, VertexType * pV)
    {
        if (edgeVertex1(pE) == pV) return (HalfEdgeType *)pE->halfedge();
        if (edgeVertex2(pE) == pV) return (HalfEdgeType *)pE->halfedge()->he_sym();
    }

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    HalfEdgeType * DynamicMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::edgeVertexHalfedge2(EdgeType * pE, VertexType * pV)
    {
        if (edgeVertex1(pE) == pV) return (HalfEdgeType *)pE->halfedge()->he_sym();
        if (edgeVertex2(pE) == pV) return (HalfEdgeType *)pE->halfedge();
    }

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    void DynamicMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::enterHalfedge(HalfEdgeType * pHe, VertexType * pV1)
    {
        VertexType * pV2 = (VertexType *)pHe->target();
        pHe->he_sym() = vertexHalfedge(pV2, pV1);

        HalfEdgeType * pHe12 = vertexHalfedge(pV1, pV2);
        HalfEdgeType * pHes = (HalfEdgeType *)pHe->he_sym();
        pV1->outHEs().push_back(pHe);

        if (pHes) {
            pHes->he_sym() = pHe;
            EdgeType * pE = (EdgeType *)pHes->edge();
            pHe->edge() = pE;
            pE->halfedge() = pHe;
        }
        else if (pHe12 != NULL)
        {
            EdgeType * pE = (EdgeType *)pHe12->edge();
            pHe->he_sym() = pHe12;
            pHe->edge() = pE;
        }
        else if (pHes == NULL && pHe12 == NULL)
        {
            EdgeType * pE = newEdge();
            pE->halfedge() = pHe;
            pHe->edge() = pE;
        }
        else
        {
            std::cout << "enter pHe error occur" << "\n";
        }
    }

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    void DynamicMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::removeHalfedge(HalfEdgeType * pHe, VertexType * pV1)
    {
        VertexType * pV2 = (VertexType *)pHe->target();
        EdgeType * pE = (EdgeType *)pHe->edge();
        HalfEdgeType * pHes = (HalfEdgeType *)pHe->he_sym();
        if (pHes) {
            pHes->he_sym() = NULL;
            if (pE->halfedge() == pHe) { pE->halfedge() = pHes; }
        }
        else {
            pE->isremoved() = true;//0708
            //deleteEdge(pE);
        }
        for (size_t i = 0; i < pV1->outHEs().size(); ++i)
        {
            if (pV1->outHEs()[i] == pHe) { pV1->outHEs().erase(i); };
        }

    }

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    void DynamicMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::destoryFace(FaceType * pF)
    {
        HalfEdgeType * pHe = (HalfEdgeType *)pF->halfedge();
        HalfEdgeType * pHef = pHe;
        VertexType * pV1 = (VertexType *)pHe->he_prev()->target();
        for (;;) {
            HalfEdgeType * pHen = (HalfEdgeType *)pHe->he_next();
            VertexType * pV1n = (VertexType *)pHe->target();
            removeHalfedge(pHe, pV1);
            deleteHalfEdge(pHe);
            pHe = pHen;
            pV1 = pV1n;
            if (pHe == pHef) break;
        }
        deleteFace(pF);
    }

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    void DynamicMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::collapseEdgeVertex(EdgeType * pE)
    {
        HalfEdgeType * pHe1 = (HalfEdgeType *)pE->halfedge();
        HalfEdgeType * pHe2 = (HalfEdgeType *)pE->halfedge()->he_sym();
        VertexType * pVs = (VertexType *)pHe1->source();
        VertexType * pVt = (VertexType *)pHe1->target();

        if (pHe1) { destoryFace(halfedgeFace(pHe1)); }
        if (pHe2) { destoryFace(halfedgeFace(pHe2)); }

        std::vector<HalfEdgeType *> arc;
        for (auto pHe : pVt->outHEs())
        {
            HalfEdgeType * temppHe = (HalfEdgeType *)pHe->he_prev();
            arc.push_back(temppHe);
        }

        for (auto pHe : arc)
        {
            removeHalfedge(pHe, (VertexType *)pHe->he_prev()->vertex());
            removeHalfedge((HalfEdgeType *)pHe->he_next(), (VertexType *)pHe->vertex());
            pHe->vertex() = pVs;
            enterHalfedge(pHe, (VertexType *)pHe->he_prev()->vertex());
            enterHalfedge((HalfEdgeType *)pHe->he_next(), (VertexType *)pHe->vertex());
        }
        deleteVertex(pVt);
    }

    template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
    void DynamicMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::collapseEdgeVertexNM(EdgeType * pE)
    {
        HalfEdgeType * pHe1 = (HalfEdgeType *)pE->halfedge();
        HalfEdgeType * pHe2 = (HalfEdgeType *)pE->halfedge()->he_sym();
        VertexType * pVs = (VertexType *)pHe1->source();

        //VertexType * pVt = pHe1 ? (VertexType *)pHe1->target() : (VertexType *)pHe2->he_prev()->target();

        VertexType * pVt = (VertexType *)pHe1->target();

        std::vector<HalfEdgeType *> around_he;
        for (auto pHe : pVs->outHEs())
        {
            if (pHe->target() == pVt)around_he.push_back((HalfEdgeType *)pHe);
        }
        for (auto pHe : pVt->outHEs())
        {
            if (pHe->target() == pVs)around_he.push_back((HalfEdgeType *)pHe);
        }

        if (around_he.size() > 2)
        {
            for (auto pHe : around_he)
            {
                HalfEdgeType * boguspHe = newHalfEdge();
                pHe->he_sym() = boguspHe;
                destoryFace((FaceType *)pHe->face());
                deleteHalfEdge(boguspHe);
            }

            std::vector<HalfEdgeType *> arc;
            for (auto pHe : pVt->outHEs())
            {
                HalfEdgeType * temppHe = (HalfEdgeType *)pHe->he_prev();
                arc.push_back(temppHe);
            }
            //for (auto pHe : pVt->outHEs()) { arc.push_back((HalfEdgeType *)pHe->he_prev()); }

            for (auto pHe : arc)
            {
                removeHalfedge(pHe, (VertexType *)pHe->he_prev()->vertex());
                removeHalfedge((HalfEdgeType *)pHe->he_next(), (VertexType *)pHe->vertex());
                pHe->vertex() = pVs;
                enterHalfedge(pHe, (VertexType *)pHe->he_prev()->vertex());
                enterHalfedge((HalfEdgeType *)pHe->he_next(), (VertexType *)pHe->vertex());
            }
            deleteVertex(pVt);
        }
        else
        {
            collapseEdgeVertex(pE);
        }

    }

}//MeshLib
#endif