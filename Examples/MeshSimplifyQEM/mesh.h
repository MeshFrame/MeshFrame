#pragma once
#include <MeshFrame/core/viewer/MeshViewer.h>
#include <MeshFrame/core/Mesh/DynamicMesh.h>
#include <MeshFrame/core/Mesh/Vertex.h>
#include <MeshFrame/core/Mesh/HalfEdge.h>
#include <MeshFrame/core/Mesh/Edge.h>
#include <MeshFrame/core/Mesh/Face.h>
#include <MeshFrame/core/Mesh/Types.h>
#include <MeshFrame/core/Mesh/Iterators2.h>

#include <string>
#include <vector>
#include <queue>
#include <stdio.h>

#include <Dense>
#include <LU>
using Eigen::Matrix3d;
using Eigen::Vector3d;

using namespace MeshLib;

class DMVertex :public CVertex
{
public:
    Matrix3d & dist() { return m_dist; };// A
    Vector3d & dn() { return m_dn; };//b
    double & d() { return m_d; };//d
protected:
    Matrix3d m_dist = Eigen::Matrix3d::Zero(3, 3);
    Vector3d m_dn = Eigen::Vector3d::Zero(3);
    double m_d = 0;
};
class DMEdge :public CEdge
{
public:
    double & cost() { return m_cost; };
    bool & sharp() { return m_sharp; };
    bool & isremoved() { return m_is_removed; };
    CPoint & new_position() { return m_new_position; };
protected:
    double m_cost = 1000;
    bool m_is_removed = false;
    bool m_sharp = false;
    CPoint m_new_position = CPoint(0, 0, 0);
};
class DMFace :public CFaceN
{
public:
    Matrix3d & dist() { return m_dist; };
    Vector3d & dn() { return m_dn; };
    double & d() { return m_d; };
    double & area() { return m_area; };
protected:
    Matrix3d m_dist = Eigen::Matrix3d::Zero(3, 3);
    Vector3d m_dn = Eigen::Vector3d::Zero(3);
    double m_d = 0;
    double m_area = 0;
};
class DMHalfEdge :public CHalfEdge {};

class EdgeMaster
{
public:
    double cost = 0;
    DMEdge* e;
};
class EdgeCompare
{
public:
    bool operator()(const EdgeMaster & e1, const EdgeMaster & e2) { return e1.cost > e2.cost; }
};

template<typename V, typename E, typename F, typename H>
class TriDMesh :public DynamicMesh<V, E, F, H>
{
};

using DMesh = TriDMesh<DMVertex, DMEdge, DMFace, DMHalfEdge>;
using Iter = CIterators<DMesh>;

class DynamicMeshM
{
public:
    DynamicMeshM();
    int simplify(int target);
    void read_obj(const char * filename) { mesh.read_obj(filename); };
    void write_obj(const char * filename) { mesh.write_obj(filename); };
protected:
    CPoint mfaceNormal(DMesh::FPtr pF);
    int initialize();
    int calcEdgeCost(DMesh::EPtr pE);
    int setPriorityQ();
    DMEdge* selectEdge();
    int edgecollapse(DMesh::EPtr pE);
    int updateLocal(DMesh::VPtr pV);
    bool legalEdgeCollapse(DMesh::EPtr pE);

    DMesh mesh;
    std::priority_queue<EdgeMaster, std::vector<EdgeMaster>, EdgeCompare> heap;
};