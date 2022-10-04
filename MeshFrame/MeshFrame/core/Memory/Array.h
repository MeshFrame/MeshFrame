#pragma once
#include <assert.h>
// A container support random access, not thread safe,
// with default pre allocated memory, in stack
// "P" in CPArray means pre allocate
// designed to contain basic numerical types, like int, double, pointer
// not safe for types that require destructor

template<typename T, int preAllocateSize >
class CPArray {
public:
	CPArray() : 
		pMem(pPreAllocated)
	{};

	~CPArray() {
		if (pMem != pPreAllocated) {
			delete[] pMem;
		}
	}

	T& operator[](const int & i) {
		assert(i < mSize);
		return pMem[i];
	}

	void push_back(const T & newMember) {
		if (mSize + 1 > mCapacity) {
			reserve(mCapacity + (int)(mCapacity / 2) + 3);
		}

		pMem[mSize] = newMember;
		++mSize;
	}

	void push_back(T && newMember) {
		if (mSize + 1 > mCapacity) {
			reserve(mCapacity + (int)(mCapacity / 2) + 3);
		}

		pMem[mSize] = std::move(newMember);
		++mSize;
	}

	// reserve memory, mention that CPArray cannot reserve memory with size less than preAllocateSize
	void reserve(const size_t & newCap) {
		if (mSize > newCap) {
			return;
		}
		if (newCap <= preAllocateSize) {
			if (pPreAllocated == pMem) {
				return;
			}
			else{
				std::move(pMem, pMem + mSize, pPreAllocated);
				delete[] pMem;
				pMem = pPreAllocated;
				mCapacity = preAllocateSize;
			}
		}
		else{
			T * newMem = new T[newCap];
			std::move(pMem, pMem + mSize, newMem);
			mCapacity = newCap;
			if (pMem != pPreAllocated) {
				delete[] pMem;
			}
			pMem = newMem;
		}

	}

	void clear() {
		mSize = 0;
	}

	void erase(size_t i) {
		for (size_t j = i; int(j) < int(mSize) - 1; ++j){
			pMem[j] = std::move(pMem[j+1]);
		}
		--mSize;
	}

	//erase form index i to i+n-1
	void eraseN(size_t i, size_t n) {
		for (size_t j = i; int(j) < int(mSize) - n; ++j) {
			pMem[j] = std::move(pMem[j + n]);
		}
		mSize -= n;
	}

	void insert(size_t i, const T & memberToInsert) {
		if (mSize + 1 > mCapacity) {
			reserve(mCapacity + (int)(mCapacity / 2) + 3);
		}
		for (size_t j = mSize; j > i + 1; --j) {
			pMem[j] = std::move(pMem[j - 1]);
		}
		pMem[i+1] = memberToInsert;
		++mSize;
	}
	void insert(size_t i, T && memberToInsert) {
		if (mSize + 1 > mCapacity) {
			reserve(mCapacity + (int)(mCapacity / 2) + 3);
		}
		for (size_t j = mSize; j > i + 1; --j) {
			pMem[j] = std::move(pMem[j-1]);
		}
		pMem[i+1] = std::move(memberToInsert);
		++mSize;
	}

	//insert n elemets after element with index i, only make rooms for those elements,
	//need to be initailized by you afterwards.
	void insertN(size_t i, int n) {	
		if (mSize + n > mCapacity) {
			reserve(mCapacity + (int)(mCapacity / 2) + n + 3);
		}
		for (size_t j = mSize + n - 1; j > n + i; --j) {
			pMem[j] = std::move(pMem[j - n]);
		}
		mSize += n;
	}

	T* begin() {
		return pMem;
	}
	T* end() {
		return pMem + mSize;
	}

	T& front() {
		return *pMem;
	}
	T& back() {
		return pMem[mSize - 1];
	}
	void pop_back() {
		--mSize;
	}

	bool has(const T & t) {
		for (size_t j = 0; j < mSize; ++j) {
			if (t == pMem[j]) {
				return true;
			}
		}
		return false;
	}

	size_t size() { return mSize; }
	size_t capacity() { return mCapacity; }
private:
	T pPreAllocated[preAllocateSize];
	T * pMem;
	size_t mSize = 0;
	size_t mCapacity = preAllocateSize;
};