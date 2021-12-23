#pragma once
#include <iterator>
/*!
*      \file MPIterator.h
*      \brief Iterators for accessing MemoryPool
*      \date 01/06/2018
*/

template<typename T>
class MemoryPool;

template<typename T>
class MPIterator : public std::iterator<std::forward_iterator_tag, T*>{
public:
	typedef	typename std::vector<char*>::iterator		MemberIter;
public:
	/*Construct*/
	MPIterator(MemberIter viter, size_t index, size_t blocksize, size_t size, std::vector<bool>& _deleteMask, size_t _memberTSize)
		:mCurrentViter(viter), mIndex(index), mBlockSize(blocksize), mSize(size), deleteMask(_deleteMask), memberTSize(_memberTSize)
	{
		bool ifFindBegin = false;
		mBlockOffset = mIndex % mBlockSize;
		while (mIndex < mSize)
		{
			mCurrentBlock = (*mCurrentViter);
			while (mBlockOffset < mBlockSize && mIndex < mSize)
			{
				if (!deleteMask[mIndex])
				{
					ifFindBegin = true;
					break;
				}
				mBlockOffset++;
				mIndex++;
			}
			if (ifFindBegin == true) break;
			mBlockOffset = 0;
			mCurrentViter++;
		}
	};
	/*!
	The pointer, gained from MemoryPool->getPointer(index)
	*/
	T	* 	value() { return (T*)(mCurrentBlock + mBlockOffset * memberTSize ); }
	/*!
	The pointer, gained from MemoryPool->getPointer(index)
	*/
	T	*	operator*() { return (T*)(mCurrentBlock + mBlockOffset * memberTSize); }
	/*!
	MPIterator prefix operator ++, goes to the next pointer
	*/
	MPIterator<T> &	operator++()
	{
		mIndex++;
		// if already meets the last element, it will not proceed;
		if (mIndex >= mSize) return *this;
		mBlockOffset++;
		if (mBlockOffset >= mBlockSize)
		{
			mBlockOffset = 0;
			mCurrentViter++;
			mCurrentBlock = (*mCurrentViter);
		}
		while (deleteMask[mIndex])
		{
			mIndex++;
			if (mIndex >= mSize) break;
			mBlockOffset++;
			if (mBlockOffset >= mBlockSize)
			{
				mBlockOffset = 0;
				mCurrentViter++;
				mCurrentBlock = (*mCurrentViter);
			}
		}
		return *this;
	}
	/*!
	MPIterator postfix operator ++, goes to the next pointer
	*/
	MPIterator<T> &	operator++(int)
	{
		if (mIndex >= mSize) return *this;
		mIndex++;
		mBlockOffset++;
		if (mBlockOffset >= mBlockSize)
		{
			mBlockOffset = 0;
			mCurrentViter++;
			mCurrentBlock = (*mCurrentViter);
		}
		if (mIndex >= mSize) return *this;
		while (deleteMask[mIndex])
		{
			mIndex++;
			mBlockOffset++;
			if (mBlockOffset >= mBlockSize)
			{
				mBlockOffset = 0;
				mCurrentViter++;
				mCurrentBlock = (*mCurrentViter);
			}
			if (mIndex >= mSize) return *this;
		}
		return *this;
	}
	/*!
	MPIterator operator ==, judge if two iterators are equal
	*/
	bool operator==(const MPIterator & otherIter) { return mIndex == otherIter.mIndex; }
	/*!
	MPIterator operator !=, judge if two iterators are not equal
	*/
	bool operator!=(const MPIterator & otherIter) { return mIndex != otherIter.mIndex; }

public:
	/*Current vector<T*> iterator*/
	MemberIter mCurrentViter;
	/*Current Index*/
	size_t mIndex;
	/*Block's size*/
	size_t mBlockSize;
	/*Max size*/
	size_t mSize;
	/*Size of a member in MemoryPool*/
	size_t mMemberSize;
	/*Current block's offset*/
	size_t mBlockOffset;
	/*Current Block*/
	char* mCurrentBlock;
	/*Deleted mask of wether a member has been deleted*/
	std::vector<bool> & deleteMask;
	/*Size of true member*/
	const size_t memberTSize;
};
