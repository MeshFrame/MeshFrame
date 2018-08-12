#include "mesh.h"

DynamicMeshM::DynamicMeshM()
{
}

CPoint DynamicMeshM::mfaceNormal(DMesh::FPtr pF)
{

    DMesh::HEPtr pHe = (DMesh::HEPtr)pF->halfedge();
    CPoint p0 = pHe->target()->point();
    CPoint p1 = pHe->he_next()->target()->point();
    CPoint p2 = pHe->he_prev()->target()->point();
    CPoint normal = (p1 - p0) ^ (p2 - p1);
    return normal;
}

int DynamicMeshM::initialize()
{
    //calc f matrix 
    for (DMesh::FPtr pF : Iter::MFIterator(&mesh))
    {
        CPoint fnormal = mfaceNormal(pF);
        pF->normal() = fnormal / fnormal.norm();
        pF->area() = 0.5*fnormal.norm();

        pF->d() = -(pF->normal()*pF->halfedge()->target()->point());
        Vector3d n;
        for (int i = 0; i < 3; i++)
        {
            n(i) = pF->normal()[i];
        }

        pF->dn() = pF->d()*n;
        pF->dist() = n * n.transpose();
    }
    //calc v matrix
    for (DMesh::VPtr pV : Iter::MVIterator(&mesh))
    {
        for (DMesh::FPtr pVF : Iter::VFIterator(pV))
        {
            pV->dist() += pVF->dist()*pVF->area();
            pV->dn() += pVF->dn()*pVF->area();
            pV->d() += pVF->d()*pVF->d()*pVF->area();
        }
    }
    //calc e cost and new pos
    for (DMesh::EPtr pE : Iter::MEIterator(&mesh))
    {
        calcEdgeCost(pE);
    }

    return 0;
}

int DynamicMeshM::calcEdgeCost(DMesh::EPtr pE)
{
    DMesh::VPtr pV1 = mesh.edgeVertex1(pE);
    DMesh::VPtr pV2 = mesh.edgeVertex2(pE);

    Matrix3d Q = pV1->dist() + pV2->dist();
    Vector3d b = pV1->dn() + pV2->dn();
    double detQ = Q.determinant();
    if (abs(detQ-0)>DBL_MIN)
    {
        Vector3d v = Q.lu().solve(-b);
        if (std::isnan(v(0))|| std::isnan(v(1)) || std::isnan(v(2)))
        {
            pE->cost() = 100;
            pE->new_position() = (pV1->point() + pV2->point()) / 2.0;
        }
        else
        {
            double d2 = pV1->d() + pV2->d();

            pE->cost() = (v.transpose()*Q*v + b.transpose()*v * 2)(0, 0) + d2;

            for (int i = 0; i < 3; i++)
            {
                pE->new_position()[i] = v(i);
            }
        }

    }
    else
    {
        pE->cost() = 100;
        pE->new_position() = (pV1->point() + pV2->point()) / 2.0;
    }
    return 0;
}

int DynamicMeshM::setPriorityQ()
{
    EdgeMaster tempE;
    for (DMesh::EPtr pE : Iter::MEIterator(&mesh))
    {
        tempE.e = pE;
        tempE.cost = pE->cost();
        heap.push(tempE);
    }
    return 0;
}

DMEdge * DynamicMeshM::selectEdge()
{
    DMEdge* pe = NULL;
    while (!heap.empty())
    {
        EdgeMaster tempE = heap.top();
        pe = tempE.e;
        heap.pop();
        if (pe == NULL)continue;

        if (pe->cost() == 100)continue;
        if (pe->isremoved() == true) continue;
        if (fabs(pe->cost() - tempE.cost) > 0.0000000001) continue;
        if (!legalEdgeCollapse(pe)) continue;
        break;
    }
    return pe;
}

int DynamicMeshM::edgecollapse(DMesh::EPtr pE)
{
    DMesh::VPtr pVs = (DMesh::VPtr)pE->halfedge()->source();
    DMesh::VPtr pVt = (DMesh::VPtr)pE->halfedge()->target();

    pVs->point() = pE->new_position();
    pVs->dist() += pVt->dist();
    pVs->dn() += pVt->dn();
    pVs->d() += pVt->d();

    DMesh::HEPtr he1 = (DMesh::HEPtr)pE->halfedge();
    DMesh::HEPtr he2 = (DMesh::HEPtr)pE->halfedge()->he_sym();

    if (he2 != NULL)
    {
        DMesh::EPtr e1 = (DMesh::EPtr)he1->he_next()->edge();
        DMesh::EPtr e2 = (DMesh::EPtr)he2->he_prev()->edge();
        e1->isremoved() = true;
        e2->isremoved() = true;
    }
    else
    {
        DMesh::EPtr e1 = (DMesh::EPtr)he1->he_next()->edge();
        e1->isremoved() = true;
    }
    pE->isremoved() = true;

    mesh.collapseEdgeVertexNM(pE);
    updateLocal(pVs);

    return 0;
}

int DynamicMeshM::updateLocal(DMesh::VPtr pV)
{
    //update local e cost
    for (DMesh::EPtr pE : Iter::VEIterator(pV))
    {
        if (pE == NULL)continue;
        if (pE->isremoved() == true) continue;
        calcEdgeCost(pE);
        EdgeMaster tempE;
        tempE.e = pE;
        tempE.cost = pE->cost();
        heap.push(tempE);
    }
    return 0;
}

int DynamicMeshM::simplify(int target)
{
    initialize();
    setPriorityQ();

    while (mesh.numFaces() > target)
    {
        int tempFnum = mesh.faces().size();
        DMEdge* e = selectEdge();
        if (e == NULL) continue;//null e occur
        if (e != NULL) {
            edgecollapse(e);
            if (tempFnum == mesh.numFaces())
            {
                std::cout << "target num is too small to preserve topology of the mesh" << "\n";
                system("pause");
                return 0;
            }
        }
        
    }
    return 0;
}

bool DynamicMeshM::legalEdgeCollapse(DMesh::EPtr pE)
{
    if (pE->halfedge()->edge() == NULL) return false;
    DMesh::VPtr ev1 = mesh.edgeVertex1(pE);
    DMesh::VPtr ev2 = mesh.edgeVertex2(pE);
    DMesh::VPtr vo1 = (DMesh::VPtr)pE->halfedge()->he_next()->target();

    DMesh::VPtr vo2 = NULL;
    if (pE->halfedge()->he_sym() != NULL)
    {
        vo2 = (DMesh::VPtr)pE->halfedge()->he_sym()->he_next()->target();
    }

    //orinted area , check face filp
    for (auto he : ev1->outHEs())
    {
        if ((DMesh::VPtr)he->target() == ev2) continue;
        if ((DMesh::VPtr)he->target() == vo2)continue;
        CPoint fv1 = he->target()->point() - pE->new_position();
        CPoint fv2 = he->he_next()->target()->point() - pE->new_position();
        CPoint fNormal = fv1 ^ fv2;
        DMesh::FPtr ftemp = (DMesh::FPtr)he->face();
        if (ftemp->normal() * fNormal < 0)
        {
            return false;
        }
    }
    for (auto he : ev2->outHEs())
    {
        if ((DMesh::VPtr)he->target() == ev1) continue;
        if ((DMesh::VPtr)he->target() == vo1) continue;
        CPoint fv1 = he->target()->point() - pE->new_position();
        CPoint fv2 = he->he_next()->target()->point() - pE->new_position();
        CPoint fNormal = fv1 ^ fv2;
        DMesh::FPtr ftemp = (DMesh::FPtr)he->face();
        if (ftemp->normal() * fNormal < 0)
        {
            return false;
        }
    }

    return true;
}
