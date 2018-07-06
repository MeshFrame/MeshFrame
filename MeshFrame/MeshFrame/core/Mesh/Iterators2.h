#pragma once
#include <iterator>
#include <algorithm>
#include <list>
#include <vector>

#include "HalfEdge.h"

namespace MeshLib {
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	struct CIteratorCore {
	private:
		typedef CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType> MeshType;
		typedef MeshType * MeshPtr;
		typedef VertexType * VPtr;
		typedef EdgeType * EPtr;
		typedef FaceType * FPtr;
		typedef HalfEdgeType * HEPtr;

	public:
		class VOutHEIterator : public std::iterator<std::forward_iterator_tag, HEPtr> {
		public:
			VOutHEIterator(const VPtr& pV) : _pV(pV), _iter(pV->outHEs().begin()) {};
			VOutHEIterator(const VPtr& pV, CHalfEdge ** iter) : _pV(pV), _iter(iter) {};

			VOutHEIterator& operator++() { ++_iter; return *this; };
			VOutHEIterator  operator++(int) { VOutHEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VOutHEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VOutHEIterator& otherIter) { return _iter != otherIter._iter; }
			HEPtr operator*() { return (HEPtr)(*_iter); }
			HEPtr value() { return (HEPtr)(*_iter); }

			VOutHEIterator begin() { return VOutHEIterator(_pV); }
			VOutHEIterator end() { return VOutHEIterator(_pV, _pV->outHEs().end()); }

			HEPtr * get() { return _iter; }
		private:
			CHalfEdge ** _iter;
			VPtr _pV;
		};

		/*
		* Iterator to access the out halfedge of a vertex in counter-clockwise direction.
		* Avalible only in manifold surface.
		* It is faster than the VoutHEIterator, so try to use this in manifold case.
		* \param pV the pointer to the vertex
		*/
		class VCcwOutHEIterator : public std::iterator<std::forward_iterator_tag, HEPtr> {
		public:
			VCcwOutHEIterator(const VPtr& pV) : _pV(pV), _pHE(MeshType::vertexMostClwOutHalfEdge(pV)) {};
			VCcwOutHEIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};

			VCcwOutHEIterator& operator++() {
				_pHE = _pHE == MeshType::vertexMostCcwOutHalfEdge(_pV) ? NULL : MeshType::vertexNextCcwOutHalfEdge(_pHE);
				return *this;
			};
			VCcwOutHEIterator  operator++(int) {
				VCcwOutHEIterator tmp(_pV, _Iter);
				_pHE = _pHE == MeshType::vertexMostCcwOutHalfEdge(_pV) ? NULL : MeshType::vertexNextCcwOutHalfEdge(_pHE);
				return tmp;
			};

			bool operator==(const VCcwOutHEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwOutHEIterator& otherIter) { return _pHE != otherIter._pHE; }
			HEPtr operator*() { return _pHE; }
			HEPtr value() { return _pHE; }

			VCcwOutHEIterator begin() { return VCcwOutHEIterator(_pV); }
			VCcwOutHEIterator end() { return  VCcwOutHEIterator(_pV, NULL); }//why null???may be a he ?

			HEPtr get() { return _pHE; }//get what??
		private:
			VPtr _pV;
			HEPtr _pHE;
		};

		/*
		* Iterator to access the out halfedge of a vertex in clockwise direction.
		* Avalible only in manifold surface.
		* It is faster than the VoutHEIterator, so try to use this in manifold case.
		* \param pV the pointer to the vertex
		*/
		class VClwInHEIterator : public std::iterator<std::forward_iterator_tag, HEPtr> {
		public:
			VClwInHEIterator(const VPtr& pV) : _pV(pV), _pHE(MeshType::vertexMostCcwInHalfEdge(pV)) {};
			VClwInHEIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};

			VClwInHEIterator& operator++() {
				_pHE = _pHE == MeshType::vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextClwInHalfEdge(_pHE);
				return *this;
			};
			VClwInHEIterator  operator++(int) {
				VClwInHEIterator tmp(pV, _pHE);
				_pHE = _pHE == MeshType::vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextClwInHalfEdge(_pHE);
				return tmp;
			};

			bool operator==(const VClwInHEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VClwInHEIterator& otherIter) { return _pHE != otherIter._pHE; }
			HEPtr operator*() { return _pHE; }
			HEPtr value() { return _pHE; }

			VClwInHEIterator begin() { return VClwInHEIterator(_pV); }
			VClwInHEIterator end() { return  VClwInHEIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VPtr _pV;
			HEPtr _pHE;
		};

		class VCcwFIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
		public:
			VCcwFIterator(const VPtr& pV) : _pV(pV), _pHE(MeshType::vertexMostClwOutHalfEdge(pV)) {};

			VCcwFIterator& operator++() {
				_pHE = _pHE == MeshType::vertexMostCcwOutHalfEdge(_pV) ? NULL : MeshType::vertexNextCcwOutHalfEdge(_pHE);
				return *this;
			};
			VCcwFIterator  operator++(int) {
				VCcwFIterator tmp(_pV, _Iter);
				_pHE = _pHE == MeshType::vertexMostCcwOutHalfEdge(_pV) ? NULL : MeshType::vertexNextCcwOutHalfEdge(_pHE);
				return tmp;
			};

			bool operator==(const VCcwFIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwFIterator& otherIter) { return _pHE != otherIter._pHE; }
			FPtr operator*() { return halfedgeFace(_pHE); }
			FPtr value() { return halfedgeFace(_pHE); }

			VCcwFIterator begin() { return VCcwFIterator(_pV); }
			VCcwFIterator end() { return  VCcwFIterator(_pV, NULL); }//why null???may be a he ?

			HEPtr get() { return _pHE; }
		private:
			VCcwFIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};
			VPtr _pV;
			HEPtr _pHE;
		};

		class VClwFIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
		public:
			VClwFIterator(const VPtr& pV) : _pV(pV), _pHE(MeshType::vertexMostCcwInHalfEdge(pV)) {};
			VClwFIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};

			VClwFIterator& operator++() {
				_pHE = _pHE == MeshType::vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextClwInHalfEdge(_pHE);
				return *this;
			};
			VClwFIterator  operator++(int) {
				VClwFIterator tmp(pV, _pHE);
				_pHE = _pHE == MeshType::vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextClwInHalfEdge(_pHE);
				return tmp;
			};

			bool operator==(const VClwFIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VClwFIterator& otherIter) { return _pHE != otherIter._pHE; }
			FPtr operator*() { return halfedgeFace(_pHE); }
			FPtr value() { return halfedgeFace(_pHE); }

			VClwFIterator begin() { return VClwFIterator(_pV); }
			VClwFIterator end() { return  VClwFIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VPtr _pV;
			HEPtr _pHE;
		};

		class VCcwEIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
		public:
			VCcwEIterator(const VPtr& pV) : _pV(pV), _pHE(MeshType::vertexMostClwOutHalfEdge(pV)) {};

			VCcwEIterator& operator++() {
				if (MeshType::isBoundary(_pV)) {
					if (_pHE == MeshType::vertexMostCcwOutHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_prev();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = MeshType::vertexNextCcwOutHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == MeshType::vertexMostCcwOutHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextCcwOutHalfEdge(_pHE);
				}
				return *this;
			};
			VCcwEIterator  operator++(int) {
				VCcwEIterator tmp(pV, pHE);
				if (isboundary(_pV)) {
					if (_pHE == MeshType::vertexMostCcwInHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_prev();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = MeshType::vertexNextCcwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == MeshType::vertexMostCcwInHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextClwInHalfEdge(_pHE);
				}
				return tmp;
			};

			bool operator==(const VCcwEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwEIterator& otherIter) { return _pHE != otherIter._pHE; }
			EPtr operator*() { return MeshType::halfedgeEdge(_pHE); }
			EPtr value() { return MeshType::halfedgeEdge(_pHE); }

			VCcwEIterator begin() { return VCcwEIterator(_pV); }
			VCcwEIterator end() { return  VCcwEIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VCcwEIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};
			VPtr _pV;
			HEPtr _pHE;
			bool reachBoundary = false;
		};

		class VClwEIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
		public:
			VClwEIterator(const VPtr& pV) : _pV(pV), _pHE(MeshType::vertexMostCcwInHalfEdge(pV)) {};

			VClwEIterator& operator++() {
				if (MeshType::isBoundary(_pV)) {
					if (_pHE == MeshType::vertexMostClwInHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_next();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = MeshType::vertexNextClwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == MeshType::vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextClwInHalfEdge(_pHE);
				}
				return *this;
			};
			VClwEIterator  operator++(int) {
				VClwEIterator tmp(pV, pHE);
				if (isboundary(_pV)) {
					if (_pHE == MeshType::vertexMostClwInHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_next();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = MeshType::vertexNextCcwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == MeshType::vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextClwInHalfEdge(_pHE);
				}
				return tmp;
			};

			bool operator==(const VClwEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VClwEIterator& otherIter) { return _pHE != otherIter._pHE; }
			EPtr operator*() { return MeshType::halfedgeEdge(_pHE); }
			EPtr value() { return MeshType::halfedgeEdge(_pHE); }

			VClwEIterator begin() { return VClwEIterator(_pV); }
			VClwEIterator end() { return  VClwEIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VClwEIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};
			VPtr _pV;
			HEPtr _pHE;
			bool reachBoundary = false;
		};

		class VVIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
		public:
			VVIterator(VPtr pV) : _pV(pV), _iter(pV->outHEs().begin()) {};

			VVIterator& operator++() {
				if (MeshType::isBoundary(_pV)) {
					if (_pLastV != NULL) {
						++_iter;
						_pLastV = NULL;
					}
					else {
						CHalfEdge * pLastHE = (*_iter)->he_prev();;
						if (pLastHE->he_sym() != NULL)
							++_iter;
						else {
							_pLastV = MeshType::halfedgeSource((HEPtr)pLastHE);
						}
					}
				}
				else {
					++_iter;
				}

				return *this;
			};
			VVIterator  operator++(int) {
				VVIterator tmp(_pV, _Iter); 
				if (MeshType::isBoundary(_pV)) {
					if (_pLastV != NULL) {
						++_iter;
						_pLastV = NULL;
					}
					else {
						CHalfEdge * pLastHE = (*_iter)->he_prev();;
						if (pLastHE->he_sym() != NULL)
							++_iter;
						else {
							_pLastV = MeshType::halfedgeSource((HEPtr)pLastHE);
						}
					}
				}
				else {
					++_iter;
				}
				return tmp;
			};

			bool operator==(const VVIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VVIterator& otherIter) { return _iter != otherIter._iter; }
			VPtr operator*() {
				if (_pLastV == NULL)
					return (VPtr)(*_iter)->target();
				else
					return _pLastV;
			}
			VPtr value() {
				if (_pLastV == NULL)
					return (VPtr)(*_iter)->target();
				else
					return _pLastV;
			}

			VVIterator begin() { return VVIterator(_pV); }
			VVIterator end() { return VVIterator(_pV, _pV->outHEs().end()); }
		private:
			VVIterator(VPtr pV, CHalfEdge ** iter) : _pV(pV), _iter(iter) {};
			CHalfEdge ** _iter;
			VPtr _pV;
			VPtr _pLastV = NULL;
		};

		class VCcwVIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
		public:
			VCcwVIterator(const VPtr& pV) : _pV(pV), _pHE(MeshType::vertexMostClwOutHalfEdge(pV)) {};

			VCcwVIterator& operator++() {
				if (MeshType::isBoundary(_pV)) {
					if (_pHE == MeshType::vertexMostCcwOutHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_prev();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = MeshType::vertexNextCcwOutHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == MeshType::vertexMostCcwOutHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextCcwOutHalfEdge(_pHE);
				}
				return *this;
			};
			VCcwVIterator  operator++(int) {
				VCcwVIterator tmp(pV, pHE);
				if (isboundary(_pV)) {
					if (_pHE == MeshType::vertexMostCcwInHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_prev();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = MeshType::vertexNextCcwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == MeshType::vertexMostCcwInHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextClwInHalfEdge(_pHE);
				}
				return tmp;
			};
			bool operator==(const VCcwVIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwVIterator& otherIter) { return _pHE != otherIter._pHE; }
			VPtr operator*() {
				if (reachBoundary)
					return MeshType::halfedgeSource(_pHE);
				else
					return MeshType::halfedgeTarget(_pHE);
			}
			VPtr value() {
				if (reachBoundary)
					return MeshType::halfedgeSource(_pHE);
				else
					return MeshType::halfedgeTarget(_pHE);
			}

			VCcwVIterator begin() { return VCcwVIterator(_pV); }
			VCcwVIterator end() { return  VCcwVIterator(_pV, NULL); }

		private:
			VCcwVIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};
			VPtr _pV;
			HEPtr _pHE;
			bool reachBoundary = false;
		};

		class VClwVIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
		public:
			VClwVIterator(const VPtr& pV) : _pV(pV), _pHE(MeshType::vertexMostCcwInHalfEdge(pV)) {};

			VClwVIterator& operator++() {
				if (MeshType::isBoundary(_pV)) {
					if (_pHE == MeshType::vertexMostClwInHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_next();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = MeshType::vertexNextClwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == MeshType::vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextClwInHalfEdge(_pHE);
				}
				return *this;
			};
			VClwVIterator  operator++(int) {
				VClwVIterator tmp(pV, pHE);
				if (isboundary(_pV)) {
					if (_pHE == MeshType::vertexMostClwInHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_next();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = MeshType::vertexNextCcwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == MeshType::vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : MeshType::vertexNextClwInHalfEdge(_pHE);
				}
				return tmp;
			};

			bool operator==(const VClwVIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VClwVIterator& otherIter) { return _pHE != otherIter._pHE; }
			VPtr operator*() {
				if (reachBoundary)
					return MeshType::halfedgeTarget(_pHE);
				else
					return MeshType::halfedgeSource(_pHE);
			}
			VPtr value() {
				if (reachBoundary)
					return MeshType::halfedgeTarget(_pHE);
				else
					return MeshType::halfedgeSource(_pHE);
			}

			VClwVIterator begin() { return VClwVIterator(_pV); }
			VClwVIterator end() { return  VClwVIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VClwVIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};
			VPtr _pV;
			HEPtr _pHE;
			bool reachBoundary = false;
		};

		class VEIterator : public std::iterator<std::forward_iterator_tag, EPtr>
		{
		public:
			VEIterator(VPtr pV) : _pV(pV), _iter(pV->outHEs().begin()) {};

			VEIterator& operator++() {
				if (MeshType::isBoundary(_pV)) {
					if (_pLastE != NULL) {
						++_iter;
						_pLastE = NULL;
					}
					else {
						CHalfEdge * pLastHE = (*_iter)->he_prev();;
						if (pLastHE->he_sym() != NULL)
							++_iter;
						else {
							_pLastE = MeshType::halfedgeEdge((HEPtr)pLastHE);
						}
					}
				}
				else {
					++_iter;
				}

				return *this;
			};
			VEIterator  operator++(int) {
				VEIterator tmp(_pV, _Iter); if (MeshType::isBoundary(_pV)) {
					if (_pLastE != NULL) {
						++_iter;
						_pLastE = NULL;
					}
					else {
						CHalfEdge * pLastHE = (*_iter)->he_prev();;
						if (pLastHE->he_sym() != NULL)
							++_iter;
						else {
							_pLastE = MeshType::halfedgeEdge((HEPtr)pLastHE);
						}
					}
				}
				else {
					++_iter;
				}
				return tmp;
			};

			bool operator==(const VEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VEIterator& otherIter) { return _iter != otherIter._iter; }

			EPtr operator*() {
				if (_pLastE == NULL)
					return  MeshType::halfedgeEdge((HEPtr)*_iter);
				else
					return _pLastE;
			}
			EPtr value() {
				if (_pLastE == NULL)
					return  MeshType::halfedgeEdge((HEPtr)*_iter);
				else
					return _pLastE;
			}

			VEIterator begin() { return VEIterator(_pV); }
			VEIterator end() { return VEIterator(_pV, _pV->outHEs().end()); }

		private:
			VEIterator(VPtr pV, CHalfEdge ** iter) : _pV(pV), _iter(iter) {};
			CHalfEdge ** _iter;
			VPtr _pV;
			EPtr _pLastE = NULL;
		};

		class VFIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
		public:
			VFIterator(VPtr pV) : _pV(pV), _iter(pV->outHEs().begin()) {};

			VFIterator& operator++() { ++_iter; return *this; };
			VFIterator  operator++(int) { VFIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VFIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VFIterator& otherIter) { return _iter != otherIter._iter; }
			FPtr operator*() { return (FPtr)(*_iter)->face(); }
			FPtr value() { return (FPtr)(*_iter)->face(); }

			VFIterator begin() { return VFIterator(_pV); }
			VFIterator end() { return VFIterator(_pV, _pV->outHEs().end()); }

			HEPtr * get() { return _iter; }
		private:
			VFIterator(VPtr pV, CHalfEdge ** iter) : _pV(pV), _iter(iter) {};
			CHalfEdge ** _iter;
			VPtr _pV;
		};

		// bad orgnized
		class FHEIterator : public std::iterator<std::forward_iterator_tag, HEPtr> {// here inherited?
		public:
			FHEIterator(FPtr pF) : _pF(pF), _pHE((HEPtr)pF->halfedge()) {};//here type cast

			FHEIterator& operator++()
			{
				_pHE = (HEPtr)_pHE->he_next();
				if (_pHE == (HEPtr)_pF->halfedge()) _pHE = NULL;
				return *this;
			};
			FHEIterator  operator++(int) { FHEIterator tmp(_pF, _pHE); ++_pHE; return tmp; };

			bool operator==(const FHEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const FHEIterator& otherIter) { return _pHE != otherIter._pHE; }
			HEPtr operator*() { return _pHE; }
			HEPtr value() { return _pHE; }

			FHEIterator begin() { return FHEIterator(_pF); }
			FHEIterator end() { return FHEIterator(_pF, NULL); }
			//FHEIterator end() { return FHIterator(_pF, (HEPtr)pF->halfedge()->he_prev())); }

			HEPtr * get() { return _pHE; }
		private:
			FHEIterator(FPtr pF, HEPtr iter) : _pF(pF), _pHE(iter) {};
			HEPtr _pHE;
			FPtr _pF;
		};

		//class FEIterator
		class FEIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
		public:
			FEIterator(FPtr pF) : _pF(pF), _pHE((HEPtr)pF->halfedge()) {};
			FEIterator(FPtr pF, HEPtr iter) : _pF(pF), _pHE(iter) {};

			FEIterator& operator++()
			{
				_pHE = (HEPtr)_pHE->he_next();
				if (_pHE == (HEPtr)_pF->halfedge()) _pHE = NULL;
				return *this;
			};
			FEIterator  operator++(int) { FEIterator tmp(_pF, _pHE); ++_pHE; return tmp; };

			bool operator==(const FEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const FEIterator& otherIter) { return _pHE != otherIter._pHE; }
			EPtr operator*() { return (EPtr)_pHE->edge(); }
			EPtr value() { return (EPtr)_pHE->edge(); }

			FEIterator begin() { return FEIterator(_pF); }
			FEIterator end() { return FEIterator(_pF, NULL); }
			//FEIterator end() { return FEIterator(_pF, (HEPtr)pF->halfedge()->he_prev()); }

			HEPtr * get() { return _pHE; }
		private:
			HEPtr _pHE;
			FPtr _pF;
		};

		//class FVItertor
		class FVIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
		public:
			FVIterator(const FPtr& pF) : _pF(pF), _pHE(MeshType::faceHalfedge(pF)) {};

			FVIterator& operator++()
			{
				_pHE = (HEPtr)_pHE->he_next();
				if (_pHE == (HEPtr)_pF->halfedge()) _pHE = NULL;
				return *this;
			};
			FVIterator  operator++(int) { FVIterator tmp(_pF, _pHE); ++_pHE; return tmp; };

			bool operator==(const FVIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const FVIterator& otherIter) { return _pHE != otherIter._pHE; }
			VPtr operator*() { return (VPtr)_pHE->vertex(); }
			VPtr value() { return (VPtr)_pHE->vertex(); }

			FVIterator begin() { return FVIterator(_pF); }
			FVIterator end() { return FVIterator(_pF, NULL); }
			HEPtr get() { return _pHE; }
		private:
			FVIterator(const FPtr& pF, const HEPtr& iter) : _pF(pF), _pHE(iter) {};
			HEPtr _pHE;
			FPtr _pF;
		};


		//we need pmesh
		class MVIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
		public:
			MVIterator(MeshPtr pM) : _pM(pM), _iter(pM->vertices().begin()) {};

			MVIterator& operator++() { ++_iter; return *this; };
			MVIterator  operator++(int) { MVIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MVIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MVIterator& otherIter) { return _iter != otherIter._iter; }
			VPtr operator*() { return *_iter; }
			VPtr value() { return *_iter; }

			MVIterator begin() { return MVIterator(_pM); }
			MVIterator end() { return MVIterator(_pM, _pM->vertices().end()); }

		private:
			MVIterator(MeshPtr pM, MPIterator<VertexType> iter) : _pM(pM), _iter(iter) {};
			MPIterator<VertexType> _iter;
			MeshPtr _pM;
		};

		class MFIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
		public:
			MFIterator(MeshPtr pM) : _pM(pM), _iter(pM->faces().begin()) {};

			MFIterator& operator++() { ++_iter; return *this; };
			MFIterator  operator++(int) { MFIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MFIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MFIterator& otherIter) { return _iter != otherIter._iter; }
			FPtr operator*() { return *_iter; }
			FPtr value() { return *_iter; }

			MFIterator begin() { return MFIterator(_pM); }
			MFIterator end() { return MFIterator(_pM, _pM->faces().end()); }

		private:
			MFIterator(MeshPtr pM, MPIterator<FaceType> iter) : _pM(pM), _iter(iter) {};
			MPIterator<FaceType> _iter;
			MeshPtr _pM;
		};

		class MEIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
		public:
			MEIterator(MeshPtr pM) : _pM(pM), _iter(pM->edges().begin()) {};

			MEIterator& operator++() { ++_iter; return *this; };
			MEIterator  operator++(int) { MEIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MEIterator& otherIter) { return _iter != otherIter._iter; }
			EPtr operator*() { return *_iter; }
			EPtr value() { return *_iter; }

			MEIterator begin() { return MEIterator(_pM); }
			MEIterator end() { return MEIterator(_pM, _pM->edges().end()); }

		private:
			MEIterator(MeshPtr pM, MPIterator<EdgeType> & iter) : _pM(pM), _iter(iter) {};
			MPIterator<EdgeType> _iter;
			MeshPtr _pM;
		};

		//class MHEIterator

	};

	template<typename MeshType>
	struct CIterators : public CIteratorCore<typename MeshType::VType, typename MeshType::EType, typename MeshType::FType, typename MeshType::HEType>
	{};
}