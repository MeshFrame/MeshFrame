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
#include <mutex>

#define DEFAULT_BLOCK_SIZE 2048

template<typename T>
class MemoryPool {
public:
	struct _memberType {
		bool deleted = false;
	};
	class CMemberT : public T, public _memberType {};
public:
	/*Default construct*/
	MemoryPool();
	~MemoryPool();
	
	/*Construct*/
	MemoryPool(size_t preAllocate, rsize_t newBlockSize = DEFAULT_BLOCK_SIZE);
	/*Class CMemberT*/
	/*Reserve memory*/
	bool reserve(size_t preAllocate);
	/*Generate a new member of type T and return its pointer*/
	T * newMember(size_t & index);
	/*Transform from members index to its pointer*/
	T * getPointer(size_t index);
	/*Deleted the memberof corresponding id, return false if it has already been deleted*/
	bool deleteMember(size_t index);
	/*Judge if current pointer has been deleted*/
	bool hasBeenDeleted(size_t index);
	/*Return the MemoryPool's maximal capacity*/
	size_t capacity();
	/*Return the MemoryPool's size*/
	size_t size();
public:
	/*Return the MemoryPool's cuurent index*/
	size_t getCurrentIndex();
	MPIterator<T> begin()
	{
		return MPIterator<T>(memoryBlockPtrVec.begin(), 0, blockSize, currentIndex);
	}
	MPIterator<T> end()
	{
		return MPIterator<T>(memoryBlockPtrVec.end(), currentIndex, blockSize, currentIndex);
	}
private:
	std::vector<CMemberT *> memoryBlockPtrVec;
	std::list<size_t> deletedMembersList;
	/*Block's size*/
	size_t blockSize = 1000;
	/*Max current member's number*/
	size_t currentIndex;
	void * getMemberPointer(size_t index);

	//std::mutex newMemberLock;
	//std::mutex deleteMemberLock;
};

template<typename T>
inline MemoryPool<T>::MemoryPool() { 
	blockSize = DEFAULT_BLOCK_SIZE;
	reserve(DEFAULT_BLOCK_SIZE);
	currentIndex = 0;
}

template<typename T>
inline MemoryPool<T>::~MemoryPool()
{
	for (int i = 0; i < memoryBlockPtrVec.size(); ++i) {
		delete[] memoryBlockPtrVec[i];
	}
}

template<typename T>
inline MemoryPool<T>::MemoryPool(size_t preAllocate, rsize_t newBlockSize)
{
	blockSize = newBlockSize;
	size_t numBlocks = preAllocate / blockSize + 1;
	currentIndex = 0;

	for (size_t i = 0; i < numBlocks; ++i) {
		CMemberT * pBlock = new CMemberT[blockSize];
		memoryBlockPtrVec.push_back(pBlock);
	}
}

template<typename T>
inline bool MemoryPool<T>::reserve(size_t preAllocate)
{
	//std::lock_guard<std::mutex> newMemberLockGuard(newMemberLock);
	//std::lock_guard<std::mutex> deleteMemberLockGuard(deleteMemberLock);

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
		CMemberT * pBlock = new CMemberT[blockSize];
		if (pBlock == NULL) return false;
		memoryBlockPtrVec.push_back(pBlock);
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
			CMemberT * pBlock = new CMemberT[blockSize];
			if (pBlock == NULL) {
				return NULL;
			}
			memoryBlockPtrVec.push_back(pBlock);
		}

		CMemberT * pNewMember = (CMemberT *)getMemberPointer(currentIndex);
		pNewMember->deleted = false;
		index = currentIndex;
		++currentIndex;
		return (T*)pNewMember;
	}
	else
	{
		index = deletedMembersList.front();
		deletedMembersList.pop_front();
		CMemberT * pNewMember = (CMemberT *)getMemberPointer(index);
		pNewMember->deleted = false;
		T * pT = (T *)pNewMember;
		pT->~T();
		*pT = T();
		return pT;
	}
}

template<typename T>
inline void * MemoryPool<T>::getMemberPointer(size_t index)
{
	//assert(index < capacity());
	int blockIndex = index / blockSize;
	int offSet = index % blockSize;
	CMemberT * pStartMember = memoryBlockPtrVec[blockIndex];
	return (pStartMember + offSet);
}

template<typename T>
inline T * MemoryPool<T>::getPointer(size_t index)
{
	assert(index <  blockSize * memoryBlockPtrVec.size());
	int blockIndex = index / blockSize;
	int offSet = index % blockSize;
	CMemberT * pStartMember = memoryBlockPtrVec[blockIndex];
	T * pT = (T *)(pStartMember + offSet);
	return pT;
}

template<typename T>
inline bool MemoryPool<T>::deleteMember(size_t index)
{
	assert(index <  blockSize * memoryBlockPtrVec.size());
	//std::lock_guard<std::mutex> newMemberLockGuard(newMemberLock);
	//std::lock_guard<std::mutex> deleteMemberLockGuard(deleteMemberLock);

	CMemberT * pMemberToDelete = (CMemberT *)getMemberPointer(index);
	if (pMemberToDelete->deleted == true) {
		return false;
	}
	else {
		pMemberToDelete->deleted = true;
		deletedMembersList.push_back(index);
		//T * pT = (T*)pMemberToDelete;
		//pT->~T();
		return true;
	}
}

template<typename T>
inline bool MemoryPool<T>::hasBeenDeleted(size_t index)
{
	assert(index <  blockSize * memoryBlockPtrVec.size());
	//std::lock_guard<std::mutex> newMemberLockGuard(deleteMemberLock);

	CMemberT * pMemberToDelete = (CMemberT *)getMemberPointer(index);
	return pMemberToDelete->deleted;
}

template<typename T>
inline size_t MemoryPool<T>::capacity()
{
	//std::lock_guard<std::mutex> newMemberLockGuard(newMemberLock);
	//std::lock_guard<std::mutex> deleteMemberLockGuard(deleteMemberLock);

	return  blockSize * memoryBlockPtrVec.size();
}

template<typename T>
inline size_t MemoryPool<T>::size()
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