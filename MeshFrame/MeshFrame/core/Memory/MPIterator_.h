#pragma once
/*!
*      \file MPIterator.h
*      \brief Iterators for accessing MemoryPool
*      \date 01/06/2018
*/

template<typename T>
class MemoryPool;

template<typename T>
class MPIterator {
public:
	typedef typename MemoryPool<T>::CMemberT				MemberType;
	typedef	typename std::vector<MemberType*>::iterator		MemberIter;
public:
	/*Construct*/
	MPIterator(MemberIter viter, size_t index, size_t blocksize, size_t size)
		:mCurrentViter(viter), mIndex(index), mBlockSize(blocksize), mSize(size)
	{
		bool ifFindBegin = false;
		mBlockOffset = mIndex % mBlockSize;
		while (mIndex < mSize)
		{
			mCurrentBlock = (*mCurrentViter);
			while (mBlockOffset < mBlockSize && mIndex < mSize)
			{
				if (mCurrentBlock[mBlockOffset].deleted == false)
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
	T	*	value() { return (T*)(mCurrentBlock + mBlockOffset); }
	/*!
	The pointer, gained from MemoryPool->getPointer(index)
	*/
	T	*	operator*() { return (T*)(mCurrentBlock + mBlockOffset); }
	/*!
	MPIterator prefix operator ++, goes to the next pointer
	*/
	T   *	operator++()
	{
		mIndex++;
		if (mIndex >= mSize) return NULL;
		mBlockOffset++;
		if (mBlockOffset >= mBlockSize)
		{
			mBlockOffset = 0;
			mCurrentViter++;
			mCurrentBlock = (*mCurrentViter);
		}
		while (mCurrentBlock[mBlockOffset].deleted == true)
		{
			mIndex++;
			if (mIndex >= mSize) return NULL;
			mBlockOffset++;
			if (mBlockOffset >= mBlockSize)
			{
				mBlockOffset = 0;
				mCurrentViter++;
				mCurrentBlock = (*mCurrentViter);
			}
		}
		return (T*)(mCurrentBlock + mBlockOffset);
	}
	/*!
	MPIterator postfix operator ++, goes to the next pointer
	*/
	T	*	operator++(int)
	{
		T * returnMember = (T*)(mCurrentBlock + mBlockOffset);
		mIndex++;
		if (mIndex >= mSize) return returnMember;
		mBlockOffset++;
		if (mBlockOffset >= mBlockSize)
		{
			mBlockOffset = 0;
			mCurrentViter++;
			mCurrentBlock = (*mCurrentViter);
		}
		while (mCurrentBlock[mBlockOffset].deleted == true)
		{
			mIndex++;
			if (mIndex >= mSize) return returnMember;
			mBlockOffset++;
			if (mBlockOffset >= mBlockSize)
			{
				mBlockOffset = 0;
				mCurrentViter++;
				mCurrentBlock = (*mCurrentViter);
			}
		}
		return returnMember;
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
	/*Current vector<MemberType*> iterator*/
	MemberIter mCurrentViter;
	/*Current Index*/
	size_t mIndex;
	/*Block's size*/
	size_t mBlockSize;
	/*Max size*/
	size_t mSize;
	/*Current block's offset*/
	size_t mBlockOffset;
	/*Current Block*/
	MemberType* mCurrentBlock;

};
