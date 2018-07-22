/*!
*      \file parser.h
*      \brief Parse the trait strings of vertex, edge, halfedge , face etc.
*
*/

#ifndef _MESHLIB_PARSER_H_
#define _MESHLIB_PARSER_H_

#include "IOFuncDef.h"

namespace Parser {
	//Return type
	enum ReturnType { StringOver = 1, TokenOut = 2, Success = 3, Failure = 4 };

	/*!
	 *	\brief CToken  class, key=(value), e.g. uv=(x y)
	*/

	class CToken
	{
	public:
		/*! key of the token */
		char mKey[MAX_TOKEN_STRING_SIZE];
		/*! value of the token */
		char mValue[MAX_TOKEN_STRING_SIZE];
	};

	/*!
	 *	\brief CParser class
	*/

	class CParser
	{
	public:
		/*!
		 *	\brief CParser constructor
		 *  \param str input string
		 */
		CParser(const char * str)
		{
			size_t len = strlen(str);
			if (len >= MAX_LINE_SIZE) {
				printf("Error: Current string's length is over MAX LINE SIZE!\n");
				return;
			}
			/*Copy string*/
			unsigned int i;
			for (i = 0; i < len; i++)
			{
				mLine[i] = str[i];
			}
			mLine[i] = 0;
			currrentPtr = mLine;
			int returnValue = nextToken();
			switch (returnValue)
			{
			case Parser::StringOver:
				printf("Error: Can't find Value! In line :  %s", mLine);
				break;
			case Parser::Failure:
				printf("Error: Can't find Value! In line :  %s", mLine);
				break;
			case Parser::TokenOut:
				printf("Error: Token's length is over! In line :  %s", mLine);
				break;
			default:
				break;
			}
		};

		/*!
		 *	CParser Destructor
		 */
		~CParser() {};

		/*!
		 *	List of tokens extracted from the string
		 */
		CToken * tokens() { return &mToken; };

		/*Jump to next token*/
		int nextToken() {
			/*Skip the blank*/
			SkipBlank();
			/*If string over, return false*/
			if (End()) return StringOver;
			/*Get the key from current line*/
			char * pKey = mKey;
			char   currentChar = NextChar();
			while (currentChar != ' ' && currentChar != '=' && !End())
			{
				*pKey++ = currentChar;
				currentChar = NextChar();
			}
			if (currentChar != '=' && currentChar != ' ')
			{
				*pKey++ = currentChar;
			}
			*pKey = 0;
			while (currentChar == ' ' && !End())
			{
				currentChar = NextChar();
			}
			if (currentChar != '=')
			{
				size_t keySize = strlen(mKey);
				if (keySize >= MAX_TOKEN_STRING_SIZE) {
					return TokenOut;
				}
				SAFE_STRCPY(mToken.mKey, mKey);
				mToken.mKey[keySize] = '\0';
				return Failure;
			}
			/*If string over, return false*/
			if (End()) return StringOver;

			currentChar = NextChar();

			while (currentChar != '(' && !End()) currentChar = NextChar();

			char * pvalue = mValue;

			while (currentChar != ')' && !End())
			{
				*pvalue++ = currentChar;
				currentChar = NextChar();
			}
			*pvalue++ = currentChar;
			*pvalue = 0;

			size_t keySize = strlen(mKey);
			if (keySize >= MAX_TOKEN_STRING_SIZE) {
				return TokenOut;
			}
			SAFE_STRCPY(mToken.mKey, mKey);
			mToken.mKey[keySize] = '\0';

			size_t valueSize = strlen(mValue);
			if (valueSize >= MAX_TOKEN_STRING_SIZE) {
				return TokenOut;
			}
			SAFE_STRCPY(mToken.mValue, mValue);
			mToken.mValue[valueSize] = '\0';

			return Success;
		}

	private:

		/*!
		 *	get the next char in the current string
		 */
		char NextChar()
		{
			char currentChar = *currrentPtr;
			currrentPtr++;
			return currentChar;
		};
		/*!
		 *	skip blank spaces
		 */
		void SkipBlank()
		{
			while (*currrentPtr == ' ')
			{
				currrentPtr++;
			}
		};
		/*!
		 *	verify if we
		 */
		bool End()
		{
			return ((*currrentPtr) == 0);
		};
		/*!
		 *	token
		 */
		CToken mToken;
		/*!
		 * The buffer to contain the string
		 */
		char mLine[MAX_LINE_SIZE];
		/*!
		 *	current key
		 */
		char mKey[MAX_TOKEN_STRING_SIZE];
		/*!
		 *	current value
		 */
		char mValue[MAX_TOKEN_STRING_SIZE];
		/*!
		 *	current pointer to the char buffer
		 */
		char * currrentPtr;
	};
}
#endif