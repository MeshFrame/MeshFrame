/*!
*      \file Strutil.h
*      \brief String utilities
*      \date 12/20/2017
*
*/

#pragma once

#include <string.h>
#include "IOFuncDef.h"

struct strutil {
	/* declaration */
	static inline  bool startsWith(const std::string& str, const std::string& substr)
	{
		return str.find(substr) == 0;
	};

	static inline bool endsWith(const std::string& str, const std::string& substr)
	{
		return (str.rfind(substr) == (str.length() - substr.length()) && str.rfind(substr) >= 0);
	};

	//Parse string functions
	/* String -> int */
	static int parseStringToInt(const char * str)
	{
		int currentInt = strtol(str, NULL, 10); //decimalism
		return currentInt;

		//int currentInt;
		//int ReturnInfo = sscanf(str, "%d", &currentInt);
		//if (ReturnInfo == 1){
		//	return currentInt;
		//}
		//else {
		//	printf("The string: \"%s\" can't be parsed to int type! It's set to zero(0) by default!");
		//	return currentInt;
		//}
	}
	/* String -> float */
	static float parseStringToFloat(const char * str)
	{
		float currentFloat = strtof(str, NULL);
		return currentFloat;

		//float currentFloat;
		//int ReturnInfo = sscanf(str, "%f", &currentFloat);
		//if (ReturnInfo == 1) {
		//	return currentFloat;
		//}
		//else {
		//	printf("The string: \"%s\" can't be parsed to float type! It's set to zero(0) by default!");
		//	return currentFloat;
		//}
	}
	/* String -> double */
	static double parseStringToDouble(const char * str)
	{
		double currentDouble = strtod(str, NULL);
		return currentDouble;

		//double currentDouble;
		//int ReturnInfo = sscanf(str, "%lf", &currentDouble);
		//if (ReturnInfo == 1) {
		//	return currentDouble;
		//}
		//else {
		//	printf("The string: \"%s\" can't be parsed to Double type! It's set to zero(0) by default!\n", str);
		//	currentDouble = 0;
		//	return currentDouble;
		//}
	}

	//Return type
	enum ReturnType { StringOver = 1, TokenOut = 2, Success = 3, Failure = 4 };

	//Delimiter class
	/*
	*	the string's delimiter
	*/
	struct Delimiter {
		/*Default construct*/
		Delimiter() : mSize(1), mDelimiter(" ") {};
		/*Construct
		\param char* delimiters the string's delimiter
		\param size_t size the delimiter's size
		*/
		Delimiter(const char * delimiters) :mDelimiter(delimiters) { mSize = strlen(delimiters); };
		const char * mDelimiter;
		size_t mSize;
	};

	//brief String Tokenizer class
	/*!
	*	String tokenizer, used to separate the whole string to tokens.
	*/
	class Tokenizer
	{
	public:
		/*Defalut Construct*/
		Tokenizer() {};
		/*Construct
		\param char* str the string which will be separted
		*/
		Tokenizer(const char * str)
			: mString(str), mOffset(0), mDelimiters() {
			stringLen = strlen(str);
		};
		/*Construct
		\param char* str the string which will be separted
		\param char* delimiters the string's delimiter
		\param size_t size the delimiter's size
		*/
		Tokenizer(const char * str, const char * delimiters)
			: mString(str), mOffset(0), mDelimiters(delimiters) {
			stringLen = strlen(str);
		};
		/*Init Tokenizer*/
		void initToken(const char * str, const char * delimiters) {
			mString = str;
			mOffset = 0;
			mDelimiters = delimiters;
			stringLen = strlen(str);
		}
		/*Jump to next token*/
		int nextToken() {
			/*Clear token*/
			SAFE_STRCPY(mToken, "");
			/*If offset >= string's length, retun StringOver*/
			if (mOffset >= stringLen) return StringOver;
			/*Find the start charater of the next token*/
			while (ifIsDelimiter(mString[mOffset]))
			{
				mOffset++;
				/*If offset >= string's length, retun StringOver*/
				if (mOffset >= stringLen) return StringOver;
			}
			mToken[0] = mString[mOffset];
			mOffset++;
			int index = 1;
			/*If offset >= string's length, retun Success*/
			if (mOffset >= stringLen) { mToken[index] = '\0'; return Success; }
			/*Find the end charater of the next token*/
			while (ifIsDelimiter(mString[mOffset]) == false)
			{
				mToken[index] = mString[mOffset];
				index++;
				mOffset++;
				/*If index >= token's size,
				*it means we can't put '\0' int token[end], return TokenOut*/
				if (index >= MAX_TOKEN_STRING_SIZE) { return TokenOut; }
				/*If offset >= string's length, retun Success*/
				if (mOffset >= stringLen) { mToken[index] = '\0'; return Success; }
			}
			mToken[index] = '\0';
			return Success;
		};
		/*Jump to next token
		\param char* delimiters the string's delimiter
		\param size_t size the delimiter's size
		*/
		int nextToken(const char * delimiters, size_t size)
		{
			/*Clear token*/
			SAFE_STRCPY(mToken, "");
			/*If offset >= string's length, retun StringOver*/
			if (mOffset >= stringLen) return StringOver;
			/*Find the start charater of the next token*/
			while (ifIsDelimiter(mString[mOffset], delimiters, size))
			{
				mOffset++;
				/*If offset >= string's length, retun StringOver*/
				if (mOffset >= stringLen) return StringOver;
			}
			mToken[0] = mString[mOffset];
			mOffset++;
			int index = 1;
			/*If offset >= string's length, retun Success*/
			if (mOffset >= stringLen) { mToken[index] = '\0'; return Success; }
			/*Find the end charater of the next token*/
			while (ifIsDelimiter(mString[mOffset], delimiters, size) == false)
			{
				mToken[index] = mString[mOffset];
				index++;
				mOffset++;
				/*If index >= token's size,
				*it means we can't put '\0' int token[end], return TokenOut*/
				if (index >= MAX_TOKEN_STRING_SIZE) { return TokenOut; }
				/*If offset >= string's length, retun Success*/
				if (mOffset >= stringLen) { mToken[index] = '\0'; return Success; }
			}
			mToken[index] = '\0';
			return Success;
		};
		/*Judge if current char is delimiter*/
		inline bool ifIsDelimiter(const char & c)
		{
			const char * currentD = mDelimiters.mDelimiter;
			size_t size = mDelimiters.mSize;
			for (int i = 0; i < size; i++)
			{
				if (c == currentD[i]) return true;
			}
			return false;
		}
		/*Judge if current char is delimiter*/
		inline bool ifIsDelimiter(const char & c, const char * delimiters, size_t size)
		{
			for (int i = 0; i < size; i++)
			{
				if (c == delimiters[i]) return true;
			}
			return false;
		}
		/*Find the firstly apparent specified string by the first and last char,
		*look for it from current position
		/param char startC, the first char
		/param char endC, the last char
		*/
		int findString(const char & startC, const char & endC)
		{
			/*Clear token*/
			SAFE_STRCPY(mToken, "");
			/*If offset >= string's length, retun StringOver*/
			if (mOffset >= stringLen) return StringOver;
			/*Find the first char of the specified string*/
			size_t currentPos = mOffset;
			while (mString[currentPos] != startC)
			{
				currentPos++;
				/*If currentPos >= string's length, retun StringOver*/
				if (currentPos >= stringLen) return StringOver;
			}
			currentPos++;
			/*If currentPos >= string's length, retun StringOver*/
			if (currentPos >= stringLen) { return StringOver; }
			int index = 0;
			while (mString[currentPos] != endC) {
				mToken[index] = mString[currentPos];
				index++;
				currentPos++;
				/*If index >= token's size,
				*it means we can't put '\0' int token[end], return TokenOut*/
				if (index >= MAX_TOKEN_STRING_SIZE) { return TokenOut; }
				/*If currentPos >= string's length, retun StringOver*/
				if (currentPos >= stringLen) { SAFE_STRCPY(mToken, ""); return StringOver; }
			}
			mToken[index] = '\0';
			return Success;
		}
		/*Find the firstly apparent specified string by the first and last char,
		*look for it from given position
		/param char startC, the first char
		/param char endC, the last char
		/param int & position, the given position
		*/
		int findString(const char & startC, const char & endC, const int & position)
		{
			/*Clear token*/
			SAFE_STRCPY(mToken, "");
			/*If given position isn't a available index, return failure*/
			if (position < 0 || position >= stringLen) return Failure;
			/*Find the first char of the specified string*/
			int currentPos = position;
			while (mString[currentPos] != startC)
			{
				currentPos++;
				/*If currentPos >= string's length, retun StringOver*/
				if (currentPos >= stringLen) return StringOver;
			}
			currentPos++;
			/*If currentPos >= string's length, retun StringOver*/
			if (currentPos >= stringLen) { return StringOver; }
			int index = 0;
			while (mString[currentPos] != endC) {
				mToken[index] = mString[currentPos];
				index++;
				currentPos++;
				/*If index >= token's size,
				*it means we can't put '\0' int token[end], return TokenOut*/
				if (index >= MAX_TOKEN_STRING_SIZE) { return TokenOut; }
				/*If currentPos >= string's length, retun StringOver*/
				if (currentPos >= stringLen) { SAFE_STRCPY(mToken, ""); return StringOver; }
			}
			mToken[index] = '\0';
			return Success;
		}
		/*Find the firstly apparent specified string by the first and last char,
		*look for it from current position
		/param char startC, the first char
		/param char endC, the last char
		*/
		int findString(const char & startC, const char & endC, char * currentString)
		{
			/*Clear token*/
			SAFE_STRCPY(currentString, "");
			/*If offset >= string's length, retun StringOver*/
			if (mOffset >= stringLen) return StringOver;
			/*Find the first char of the specified string*/
			size_t currentPos = mOffset;
			while (mString[currentPos] != startC)
			{
				currentPos++;
				/*If currentPos >= string's length, retun StringOver*/
				if (currentPos >= stringLen) return StringOver;
			}
			currentPos++;
			/*If currentPos >= string's length, retun StringOver*/
			if (currentPos >= stringLen) { return StringOver; }
			int index = 0;
			while (mString[currentPos] != endC) {
				currentString[index] = mString[currentPos];
				index++;
				currentPos++;
				/*If index >= token's size,
				*it means we can't put '\0' int token[end], return TokenOut*/
				if (index >= MAX_TOKEN_STRING_SIZE) { return TokenOut; }
				/*If currentPos >= string's length, retun StringOver*/
				if (currentPos >= stringLen) { return StringOver; }
			}
			currentString[index] = '\0';
			return Success;
		}
		/*Return current token*/
		const char * getToken() const { return mToken; };
		/*
		* to reset the tokenizer. After reset it, the tokenizer can get
		* the tokens from the first token.
		*/
		void reset() { mOffset = 0; };

	protected:
		size_t mOffset;
		const char * mString;
		size_t stringLen;
		char mToken[MAX_TOKEN_STRING_SIZE];
		Delimiter mDelimiters;
	};

};