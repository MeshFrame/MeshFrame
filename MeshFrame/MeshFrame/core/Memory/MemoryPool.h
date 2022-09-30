#pragma once
/*!
*      \file MemoryPool.h
*      \brief A simple implation of memory pool, relying on std::vector and its index;
*
*      The access and the delete of members of memory pool rely the index, which is a size_t variation.
*
*/

#include <vector>
#include <list>
#include <assert.h>
#include "./MPIterator.h"

#define DEFAULT_BLOCK_SIZE 2048

template<typename T>
class MemoryPool {
public:
	/*Default construct*/
	MemoryPool();
	~MemoryPool();
	
	/*Construct*/
	MemoryPool(size_t preAllocate, size_t newBlockSize = DEFAULT_BLOCK_SIZE);
	/*Reserve memory*/
	bool reserve(size_t preAllocate);
	/*Generate a new member of type T and return its pointer*/
	T * newMember(size_t & index);
	/*Generate a new member of type T and return its pointer, and initialize it with initial value*/
	T * newMember(size_t & index, const T & initialVal);
	/*Transform from members index to its pointer*/
	T* getPointer(const size_t& index);
	const T * getPointer(const size_t & index) const;
	T & front() {
		int i = 0;
		while (deleteMask[i] && i < size()) {
			++i;
		};
		if (i == size())
			return *getPointer(0);
		else
			return *getPointer(i);
	};

	T& operator[] (int i) {
		return  *getPointer(i);
	}
	const T& operator[] (int i) const {
		return  *getPointer(i);
	}
	/*Deleted the memberof corresponding id, return false if it has already been deleted*/
	bool deleteMember(size_t index);
	/*Judge if current pointer has been deleted*/
	bool hasBeenDeleted(size_t index);
	/*Return the MemoryPool's maximal capacity*/
	size_t capacity();
	/*Return the MemoryPool's size*/
	size_t size() const;
	/*Return the MemoryPool's one single block's size*/
	size_t getBlockSize() { return blockSize; };

	// non-copyable
	MemoryPool(const MemoryPool&) = delete;
public:
	/*Return the MemoryPool's cuurent index*/
	size_t getCurrentIndex();
	MPIterator<T> begin()
	{
		return MPIterator<T>(memoryBlockPtrVec.begin(), 0, blockSize, currentIndex, deleteMask, memberTSize);
	}
	MPIterator<T> end()
	{
		return MPIterator<T>(memoryBlockPtrVec.end(), currentIndex, blockSize, currentIndex, deleteMask, memberTSize);
	}
private:
	std::vector<char*> memoryBlockPtrVec;
	std::list<size_t> deletedMembersList;
	/*Block's size*/
	const size_t blockSize = DEFAULT_BLOCK_SIZE;
	/*Max current member's number*/
	size_t currentIndex;
	void * getMemberPointer(size_t index);
	/*masks on which member has been deleted*/
	std::vector<bool> deleteMask;
	/*size of true member type, in other word, you can cast MemoryPool<Son> to MemoryPool<Father>,
	* and memberTSize is still sizeof(Son), and you can iterate MemoryPool<Son> pool as MemoryPool<Father>.
	*/
	const size_t memberTSize;

	//std::mutex newMemberLock;
	//std::mutex deleteMemberLock;
};

template<typename T>
inline MemoryPool<T>::MemoryPool() : memberTSize(sizeof(T)) , blockSize(DEFAULT_BLOCK_SIZE)
{ 
	
	reserve(DEFAULT_BLOCK_SIZE);
	deleteMask.reserve(DEFAULT_BLOCK_SIZE);
	currentIndex = 0;
}

template<typename T>
inline MemoryPool<T>::~MemoryPool()
{
	for (int i = 0; i < memoryBlockPtrVec.size(); ++i) {
		delete[] (T*)memoryBlockPtrVec[i];
	}
}

template<typename T>
inline MemoryPool<T>::MemoryPool(size_t preAllocate, size_t newBlockSize)
	: memberTSize(sizeof(T)),
	blockSize(newBlockSize)
{
	size_t numBlocks = preAllocate / blockSize + 1;
	currentIndex = 0;
	deleteMask.reserve(preAllocate);

	for (size_t i = 0; i < numBlocks; ++i) {
		T * pBlock = new T[blockSize];
		memoryBlockPtrVec.push_back((char *)pBlock);
	}
}

template<typename T>
inline bool MemoryPool<T>::reserve(size_t preAllocate)
{
	//std::lock_guard<std::mutex> newMemberLockGuard(newMemberLock);
	//std::lock_guard<std::mutex> deleteMemberLockGuard(deleteMemberLock);
	deleteMask.reserve(preAllocate);

	if (currentIndex - deletedMembersList.size() > preAllocate) {
		return false;
	}

	if (blockSize * memoryBlockPtrVec.size() > preAllocate) {
		return true;
	}

	//blockSize = newBlockSize;
	size_t numBlocks = (preAllocate - blockSize * memoryBlockPtrVec.size())/ blockSize + 1;
	//currentIndex = 0;

	for (size_t i = 0; i < numBlocks; ++i) {
		T * pBlock = new T[blockSize];
		if (pBlock == NULL) return false;
		memoryBlockPtrVec.push_back((char *)pBlock);
	}

	return true;
}

template<typename T>
inline T * MemoryPool<T>::newMember(size_t & index)
{
	//std::lock_guard<std::mutex> newMemberLockGuard(newMemberLock);
	//std::lock_guard<std::mutex> deleteMemberLockGuard(deleteMemberLock);

	if (deletedMembersList.empty()) {
		if (currentIndex >= memoryBlockPtrVec.size() * blockSize) {
			T * pBlock = new T[blockSize];
			if (pBlock == NULL) {
				return NULL;
			}
			memoryBlockPtrVec.push_back((char*)pBlock);
		}

		T * pNewMember = (T *)getMemberPointer(currentIndex);
		index = currentIndex;
		++currentIndex;
		deleteMask.push_back(false);
		return (T*)pNewMember;
	}
	else
	{
		index = deletedMembersList.front();
		deletedMembersList.pop_front();
		T * pNewMember = (T *)getMemberPointer(index);
		deleteMask[index] = false;
		T * pT = (T *)pNewMember;
		//pT->~T();
		*pT = T();
		return pT;
	}
}

template<typename T>
inline T * MemoryPool<T>::newMember(size_t & index, const T & initialVal)
{
	//std::lock_guard<std::mutex> newMemberLockGuard(newMemberLock);
	//std::lock_guard<std::mutex> deleteMemberLockGuard(deleteMemberLock);

	if (deletedMembersList.empty()) {
		if (currentIndex >= memoryBlockPtrVec.size() * blockSize) {
			T * pBlock = new T[blockSize];
			if (pBlock == NULL) {
				return NULL;
			}
			memoryBlockPtrVec.push_back((char*)pBlock);
		}

		T * pNewMember = (T *)getMemberPointer(currentIndex);
		index = currentIndex;
		++currentIndex;
		deleteMask.push_back(false);
		*pNewMember = initialVal;
		return (T*)pNewMember;
	}
	else
	{
		index = deletedMembersList.front();
		deletedMembersList.pop_front();
		T * pNewMember = (T *)getMemberPointer(index);
		deleteMask[index] = false;
		T * pT = (T *)pNewMember;
		//pT->~T();
		*pT = initialVal;
		return pT;
	}
}

template<typename T>
inline void * MemoryPool<T>::getMemberPointer(size_t index)
{
	//assert(index < capacity());
	size_t blockIndex = index / blockSize;
	size_t offSet = index % blockSize;
	char * pStartMember = memoryBlockPtrVec[blockIndex];
	return (pStartMember + offSet * memberTSize);
}

template<typename T>
inline T * MemoryPool<T>::getPointer(const size_t & index)
{
	assert(index <  blockSize * memoryBlockPtrVec.size());
	//int blockIndex = index / blockSize;
	//int offSet = index % blockSize;
	char * & pStartMember = memoryBlockPtrVec[index / blockSize];
	T * pT = (T *)(pStartMember + (index % blockSize) * memberTSize);
	return pT;
}

template<typename T>
inline const T* MemoryPool<T>::getPointer(const size_t& index) const
{
	assert(index < blockSize* memoryBlockPtrVec.size());
	//int blockIndex = index / blockSize;
	//int offSet = index % blockSize;
	const char* pStartMember = memoryBlockPtrVec[index / blockSize];
	const T* pT = (const T*)(pStartMember + (index % blockSize) * memberTSize);
	return pT;
}

template<typename T>
inline bool MemoryPool<T>::deleteMember(size_t index)
{
	// Access exceeds boundary
	assert(index <  blockSize * memoryBlockPtrVec.size());
	//std::lock_guard<std::mutex> newMemberLockGuard(newMemberLock);
	//std::lock_guard<std::mutex> deleteMemberLockGuard(deleteMemberLock);

	T * pMemberToDelete = (T *)getMemberPointer(index);
	if (deleteMask[index]) {
		return false;
	}
	else {
		deleteMask[index] = true;
		deletedMembersList.push_back(index);
		return true;
	}
}

template<typename T>
inline bool MemoryPool<T>::hasBeenDeleted(size_t index)
{
	assert(index <  blockSize * memoryBlockPtrVec.size());
	//std::lock_guard<std::mutex> newMemberLockGuard(deleteMemberLock);

	T * pMemberToDelete = (T *)getMemberPointer(index);
	return deleteMask[index];
}

template<typename T>
inline size_t MemoryPool<T>::capacity()
{
	//std::lock_guard<std::mutex> newMemberLockGuard(newMemberLock);
	//std::lock_guard<std::mutex> deleteMemberLockGuard(deleteMemberLock);

	return  blockSize * memoryBlockPtrVec.size();
}

template<typename T>
inline size_t MemoryPool<T>::size() const
{
	//std::lock_guard<std::mutex> newMemberLockGuard(newMemberLock);
	//std::lock_guard<std::mutex> deleteMemberLockGuard(deleteMemberLock);

	return currentIndex - deletedMembersList.size();
}

template<typename T>
inline size_t MemoryPool<T>::getCurrentIndex()
{
	//std::lock_guard<std::mutex> newMemberLockGuard(newMemberLock);
	//std::lock_guard<std::mutex> deleteMemberLockGuard(deleteMemberLock);

	return currentIndex;
}