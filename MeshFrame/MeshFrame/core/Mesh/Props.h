#ifndef _PROPS_H_
#define _PROPS_H_
#include <vector>
#define PROP_POOL_DEFAULT_BLOCK_SIZE 2048

#define MAKE_PROPHANDLE(TARGET) \
template<typename T> \
class TARGET##PropHandle : public BasicPropHandle { \
public:\
	TARGET##PropHandle() {};\
	TARGET##PropHandle(const T & initialVal) : typeInitialVal(initialVal) { toInitialize = true;};\
	void setTypeInitialVal(const T & initialVal){\
		typeInitialVal = initialVal; \
		toInitialize = true;\
	}\
	void initializePropMember(void * pP) {\
		*((T*)pP) = typeInitialVal;\
	}; \
	void initializePropMember(size_t index) {\
		(*((PropPool<T> *)pPropPool))[index] = typeInitialVal;\
	}; \
	void destructProp(){\
		PropPool<T> * pTPropPool = (PropPool<T> *)pPropPool;\
		delete pTPropPool;\
	}\
	private:\
	T typeInitialVal; \
};

#define MAKE_PROP_OF(TARGET) \
Props m##TARGET##Props; \
public:\
template<typename T> \
void add##TARGET##Prop(TARGET##PropHandle<T> & prop){ \
	if (prop.pPropPool != NULL){\
		printf("Error! This prop handle has already been added to mesh!\n");\
		throw -1;\
	}\
	PropPool<T> * pPool = new PropPool<T>(m##TARGET##Container.capacity(), m##TARGET##Container.getBlockSize());\
	prop.pPropPool = (void*)pPool;\
	if(prop.needInitialize()){\
		for (int i = 0; i < m##TARGET##Container.getCurrentIndex(); ++i){\
			if (m##TARGET##Container.hasBeenDeleted(i)) continue;\
			prop.initializePropMember(i);\
		}\
	}\
	prop.propIdx = m##TARGET##Props.size(); \
	m##TARGET##Props.push_back(&prop);\
};\
template<typename T> \
void add##TARGET##Prop(TARGET##PropHandle<T> & prop, const T & initialVal){ \
	if (prop.pPropPool != NULL){\
		printf("Error! This prop handle has already been added to mesh!\n");\
		throw -1;\
	}\
	prop.setTypeInitialVal(initialVal);\
	PropPool<T> * pPool = new PropPool<T>(m##TARGET##Container.capacity(), m##TARGET##Container.getBlockSize());\
	prop.pPropPool = (void*)pPool;\
	for (int i = 0; i < m##TARGET##Container.getCurrentIndex(); ++i){\
		if (m##TARGET##Container.hasBeenDeleted(i)) continue;\
		prop.initializePropMember(i);\
	}\
	prop.propIdx = m##TARGET##Props.size(); \
	m##TARGET##Props.push_back(&prop);\
};\
template<typename T> \
T& get##TARGET##Prop(const TARGET##PropHandle<T> & prop, TARGET##Ptr  ptr){\
	assert(prop.pPropPool != NULL);\
	PropPool<T> * pPropPool = (PropPool<T>*)prop.pPropPool; \
	return (*pPropPool)[ptr->index()];\
};\
template<typename T> \
T& g##TARGET##P(const TARGET##PropHandle<T> & prop, TARGET##Ptr  ptr){\
	assert(prop.pPropPool != NULL);\
	PropPool<T> * pPropPool = (PropPool<T>*)prop.pPropPool; \
	return (*pPropPool)[ptr->index()];\
};\
template<typename T> \
void remove##TARGET##Prop(TARGET##PropHandle<T> & prop){\
	assert(prop.propIdx != -1);\
	PropPool<T> *pPropPool = (PropPool<T>*)prop.pPropPool;\
	delete pPropPool;\
	prop.pPropPool = NULL;\
	m##TARGET##Props.erase(m##TARGET##Props.begin() + prop.propIdx);\
	for (int i = prop.propIdx; i < m##TARGET##Props.size(); ++i){\
		m##TARGET##Props[i]->propIdx -= 1;\
	}\
};


namespace MeshLib {
	template<typename T>
	class PropPool {
	public:
		PropPool(size_t preAllocate = PROP_POOL_DEFAULT_BLOCK_SIZE, size_t newBlockSize = PROP_POOL_DEFAULT_BLOCK_SIZE) : blockSize(newBlockSize){
			size_t numBlocks = preAllocate / blockSize + 1;

			for (size_t i = 0; i < numBlocks; ++i) {
				T * pBlock = new T[blockSize];
				memoryBlockPtrVec.push_back(pBlock);
			}
		};

		~PropPool() {
			for (int i = 0; i < memoryBlockPtrVec.size(); ++i) {
				delete[] memoryBlockPtrVec[i];
			}
		}

		void reserve(size_t reserveSize) {
			if (blockSize * memoryBlockPtrVec.size() > reserveSize) {
				return;
			}

			size_t numBlocks = (reserveSize - blockSize * memoryBlockPtrVec.size()) / blockSize + 1;

			for (size_t i = 0; i < numBlocks; ++i) {
				T * pBlock = new T[blockSize];
				if (pBlock == NULL) return;
				memoryBlockPtrVec.push_back(pBlock);
			}

			return;
		}

		T & operator[] (size_t index) {
			if (index >= blockSize * memoryBlockPtrVec.size()) {
				reserve(index + 1);
			}
			size_t blockIndex = index / blockSize;
			size_t offSet = index % blockSize;
			T * pStartMember = memoryBlockPtrVec[blockIndex];
			return *(pStartMember + offSet);
		}

		T* getPointer(size_t index) {
			if (index >= blockSize * memoryBlockPtrVec.size()) {
				reserve(index + 1);
			}
			int blockIndex = index / blockSize;
			int offSet = index % blockSize;
			T * pStartMember = memoryBlockPtrVec[blockIndex];
			return pStartMember + offSet;
		};

		// non-copyable
		PropPool(const PropPool&) = delete;
	private:
		const size_t blockSize;
		std::vector<T*> memoryBlockPtrVec;
	};

	class BasicPropHandle
	{
	public:
		BasicPropHandle() {};
		size_t propIdx = 0;
		void * pPropPool = NULL;
		bool needInitialize() { return toInitialize; };
		virtual void initializePropMember(void * pP) {};
		virtual void initializePropMember(size_t index) {};
		virtual void destructProp() {};
	private:
		// Handle is not copyable
		//BasicPropHandle(const BasicPropHandle &);
		//BasicPropHandle &operator=(const BasicPropHandle &);
	protected:
		bool toInitialize = false;
	};

	MAKE_PROPHANDLE(V);
	MAKE_PROPHANDLE(E);
	MAKE_PROPHANDLE(F);
	MAKE_PROPHANDLE(HE);

	typedef std::vector<BasicPropHandle *> Props;



}
#endif // !_PROPS_H_
