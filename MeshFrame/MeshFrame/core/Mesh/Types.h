/*!
*      \file Vertex.h
*      \brief Derivative classes from CVertex, CFace, and CEdge, containing
*			information of normal, color, and uv
*	   \Version 2.0
*	   \Update 31/05/2018
*/
#ifndef _TYPES_H_
#define _TYPES_H_

#include "Color.h"
#include "../Parser/parser.h"

#define HAS_FIELD_UV \
	protected : \
		CPoint2 m_uv; \
	public : \
		CPoint2 & uv() { return m_uv; }; \
		virtual bool hasUV() { return true; }; 
		
#define LOAD_UV \
	if (strcmp((key), "uv") == 0) { \
		sscanf((val), "(%lf %lf)", &m_uv[0], &m_uv[1]); \
	}

#define WRITE_UV \
	SAFE_SPRINT(_str,MAX_TRAIT_STRING_SIZE, "uv=(%lf %lf) ", m_uv[0], m_uv[1]); \
	SAFE_STRCAT(str,MAX_TRAIT_STRING_SIZE, _str);

#define HAS_FIELD_COLOR \
	protected: \
		ColorUnion m_color; \
	public: \
		ColorUnion & color() { return m_color; }; \
		virtual bool hasColor() { return true; };

#define LOAD_COLOR \
	if (strcmp((key), "rgb") == 0) { \
		sscanf((val), "(%f %f %f)", &(m_color.r), &(m_color.g), &(m_color.b)); \
	}

#define WRITE_COLOR \
	SAFE_SPRINT(_str, MAX_TRAIT_STRING_SIZE, "rgb=(%f %f %f) ", m_color.r, m_color.g, m_color.b); \
	SAFE_STRCAT(str,MAX_TRAIT_STRING_SIZE, _str);

#define HAS_FIELD_NORMAL \
	protected: \
		CPoint m_normal; \
	public: \
		CPoint & normal() { return m_normal; }; \
		virtual bool hasNormal() { return true; };

#define LOAD_NORMAL \
	if (strcmp((key), "normal") == 0) { \
		sscanf((val), "(%lf %lf %lf)", &m_normal[0], &m_normal[1], &m_normal[2]); \
	}

#define WRITE_NORMAL \
	SAFE_SPRINT(_str, MAX_TRAIT_STRING_SIZE, "normal=(%lf %lf %lf) ", m_normal[0], m_normal[1], m_normal[2]); \
	SAFE_STRCAT(str,MAX_TRAIT_STRING_SIZE, _str);

namespace MeshLib {

	class CVertexUV : public CVertex
	{
		HAS_FIELD_UV;
	public:
		void _from_string_default(const char * str){
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;
				LOAD_UV;
			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_UV;
		}
	};

	class CVertexRGB : public CVertex
	{
		HAS_FIELD_COLOR;
	public:
		void _from_string_default(const char * str) {
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;

				LOAD_COLOR;

			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_COLOR;
		}
	};

	class CVertexN : public CVertex
	{
		HAS_FIELD_NORMAL;
	public:	
		void _from_string_default(const char * str) {
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;

				LOAD_NORMAL;

			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_NORMAL;
		}
	};
	class CVertexNUV : public CVertex
	{
		HAS_FIELD_UV;
		HAS_FIELD_NORMAL;
	public:
		void _from_string_default(const char * str) {
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;

				LOAD_UV;
				LOAD_NORMAL;
			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_NORMAL;
			WRITE_UV;
		}
	};
	class CVertexUVRGB : public CVertex
	{
		HAS_FIELD_UV;
		HAS_FIELD_COLOR;
	public:
		void _from_string_default(const char * str) {
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;

				LOAD_UV;
				LOAD_COLOR;
			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_COLOR;
			WRITE_UV;
		}
	};
	class CVertexNRGB : public CVertex
	{
		HAS_FIELD_NORMAL;
		HAS_FIELD_COLOR;
	public:
		void _from_string_default(const char * str) {
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;

				LOAD_NORMAL;
				LOAD_COLOR;
			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_COLOR;
			WRITE_NORMAL;
		}
	};
	class CVertexNUVRGB : public CVertex
	{
		HAS_FIELD_UV;
		HAS_FIELD_NORMAL;
		HAS_FIELD_COLOR;
	public:
		void _from_string_default(const char * str) {
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;

				LOAD_UV;
				LOAD_NORMAL;
				LOAD_COLOR;
			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_UV;
			WRITE_COLOR;
			WRITE_NORMAL;
		}
	};


	class CFaceRGB : public CFace 
	{
		HAS_FIELD_COLOR;
	public:
		void _from_string_default(const char * str) {
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;

				LOAD_COLOR;
			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_COLOR;
		}
	};

	class CFaceN : public CFace
	{
		HAS_FIELD_NORMAL;
	public:
		void _from_string_default(const char * str) {
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;

				LOAD_NORMAL;
			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_NORMAL;
		}
	};

	class CFaceNRGB : public CFace
	{
		HAS_FIELD_COLOR;
		HAS_FIELD_NORMAL;
	public:
		void _from_string_default(const char * str) {
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;

				LOAD_COLOR;
				LOAD_NORMAL;
			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_COLOR;
			WRITE_NORMAL;
		}
	};

	class CEdgeRGB : public CEdge
	{
		HAS_FIELD_COLOR;
	public:
		void _from_string_default(const char * str) {
			Parser::CParser parser(str);
			do
			{
				Parser::CToken * pToken = parser.tokens();
				char * key = pToken->mKey;
				char * val = pToken->mValue;

				LOAD_COLOR;
			} while (parser.nextToken() == Parser::Success);

			 
		};

		void _to_string_default(char * str) {
			char _str[MAX_TRAIT_STRING_SIZE];
			WRITE_COLOR;
		}
	};
}

#endif // !_TYPES_H_
