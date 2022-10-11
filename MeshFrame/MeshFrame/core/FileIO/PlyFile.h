/*

Header for PLY polygon files.

- Greg Turk, March 1994

A PLY file contains a single polygonal _object_.

An object is composed of lists of _elements_.  Typical elements are
vertices, faces, edges and materials.

Each type of element for a given object has one or more _properties_
associated with the element type.  For instance, a vertex element may
have as properties three floating-point values x,y,z and three unsigned
chars for red, green and blue.

---------------------------------------------------------------

Copyright (c) 1994 The Board of Trustees of The Leland Stanford
Junior University.  All rights reserved.

Permission to use, copy, modify and distribute this software and its
documentation for any purpose is hereby granted without fee, provided
that the above copyright notice and this permission notice appear in
all copies of this software and that you do not sell the software.

THE SOFTWARE IS PROVIDED "AS IS" AND WITHOUT WARRANTY OF ANY KIND,
EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.

*/
/*
Update 2018/3/2 by B.Xiao
Reason: To better fit the meshlib library
*/

#ifndef __PLYFILE_H__
#define __PLYFILE_H__

#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS  
#define _CRT_SECURE_NO_WARNINGS
#endif 
#endif

#define USE_PLY_WRAPPER 1

#ifndef WIN32
#define _strdup _strdup
#endif

#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <iostream>
#include "../Parser/IOFuncDef.h"

enum PLY_FILE_TYPE{ 
	PLY_ASCII			= 1,
	PLY_BINARY_BE		= 2,
	PLY_BINARY_LE		= 3,
	PLY_BINARY_NATIVE	= 4
};

/* scalar data types supported by PLY format */
enum PLY_DATA_TYPE{
	PLY_START_TYPE	= 0,
	PLY_CHAR		= 1,
	PLY_SHORT		= 2,
	PLY_INT			= 3,
	PLY_UCHAR		= 4,
	PLY_USHORT		= 5,
	PLY_UINT		= 6,
	PLY_FLOAT		= 7,
	PLY_DOUBLE		= 8,
	PLY_INT_8		= 9,
	PLY_UINT_8		= 10,
	PLY_INT_16		= 11,
	PLY_UINT_16		= 12,
	PLY_INT_32		= 13,
	PLY_UINT_32		= 14,
	PLY_FLOAT_32	= 15,
	PLY_FLOAT_64	= 16,
	PLY_END_TYPE	= 17
};

enum PLY_NAME_MARK {
	PLY_X			= 0,
	PLY_Y			= 1,
	PLY_Z			= 2,
	PLY_RED			= 3,
	PLY_GREEN		= 4,
	PLY_BLUE		= 5,
	PLY_NX			= 6,
	PLY_NY			= 7,
	PLY_NZ			= 8,
	PLY_U			= 9,
	PLY_V			= 10,
	PLY_VERTEXS		= 11,
	PLY_OTHER		= 12
};

typedef struct PlyProperty {    /* description of a property */
	char propName[MAX_TOKEN_STRING_SIZE];                           /* property name */
	int nameMark;
	int externalType;                    /* file's data type */

	bool isList;                          /* 1 = list, 0 = scalar */
	int countType;                   /* file's count type */
} PlyProperty;

typedef struct PlyElement {     /* description of an element */
	char			elemName[MAX_TOKEN_STRING_SIZE];                   /* element name */
	int				eleNum;												 /* number of elements in this object */
	int				propNum;										/* number of properties for this element */
	PlyProperty**	propList;							/* list of properties in the file */
} PlyElement;

typedef struct PlyOtherProperty {
	char	propName[MAX_TOKEN_STRING_SIZE];
	bool	ifMalloc;
	int		offset;

	int externalType;

	bool isList;
	int countType;
	int listCount;
	
	int* intValues;
	unsigned int* uintValues;
	double* doubleValues;
}PlyOtherProperty;

typedef struct PlyOtherElem {
	char				elemName[MAX_TOKEN_STRING_SIZE];
	int					elemCount;

	int					propNum;
	PlyOtherProperty**	propList;
}PlyOtherElem;

typedef struct PlyOtherData {
	int				fileType;
	int				elemNum;               
	PlyOtherElem**	elemList;
} PlyOtherData;

typedef struct PlyFile {        /* description of PLY file */
	FILE *fp;                     /* file pointer */
	int file_type;                /* ascii or binary */
	float version;                /* version number of file */
	int nelems;                   /* number of elements of object */
	PlyElement **elems;           /* list of elements */
	int num_comments;             /* number of comments */
	char **comments;              /* list of comments */
	int num_obj_info;             /* number of items of object information */
	char **obj_info;              /* list of object info items */
	PlyElement *which_elem;       /* which element we're currently writing */
	PlyOtherData *other_elems;   /* "other" elements from a PLY file */
} PlyFile;

#define myalloc(mem_size) my_alloc((mem_size), __LINE__, __FILE__)
#ifndef ALLOCN
#define REALLOCN(PTR,TYPE,OLD_N,NEW_N)							\
{										\
	if ((OLD_N) == 0)                                           		\
{   ALLOCN((PTR),TYPE,(NEW_N));}                            		\
	else									\
{								    		\
	(PTR) = (TYPE *)realloc((PTR),(NEW_N)*sizeof(TYPE));			\
	if (((PTR) == NULL) && ((NEW_N) != 0))					\
{									\
	fprintf(stderr, "Memory reallocation failed on line %d in %s\n", 	\
	__LINE__, __FILE__);                             		\
	fprintf(stderr, "  tried to reallocate %d->%d\n",       		\
	(OLD_N), (NEW_N));                              		\
	exit(-1);								\
}									\
	if ((NEW_N)>(OLD_N))							\
	memset((char *)(PTR)+(OLD_N)*sizeof(TYPE), 0,			\
	((NEW_N)-(OLD_N))*sizeof(TYPE));				\
}										\
}

#define  ALLOCN(PTR,TYPE,N) 					\
{ (PTR) = (TYPE *) calloc(((unsigned)(N)),sizeof(TYPE));\
	if ((PTR) == NULL) {    				\
	fprintf(stderr, "Memory allocation failed on line %d in %s\n", \
	__LINE__, __FILE__);                           \
	exit(-1);                                             \
	}							\
}

#define FREE(PTR)  { free((PTR)); (PTR) = NULL; }
#endif

class PlyFileReader
{
public:
	PlyFileReader(bool hasColor, bool hasNormal, bool hasUV) :native_binary_type(-1), types_checked(0) 
	{
		if (hasColor)
			vertexType[0] = true;
		else
			vertexType[0] = false;
		if (hasNormal)
			vertexType[1] = true;
		else
			vertexType[1] = false;
		if (hasUV)
			vertexType[2] = true;
		else
			vertexType[2] = false;
	};
	~PlyFileReader() {};
	/* delcaration of routines */
	PlyFile *ply_write(FILE *, int, const char **, int);
	PlyFile *ply_open_for_writing(const char *, int, const char **, int, float *);
	void ply_describe_property(PlyFile *, const char *, int);
	void ply_element_count(PlyFile *, const char *, int);
	void ply_header_complete(PlyFile *);
	void ply_put_element_setup(PlyFile *, const char *);
	void ply_put_element(PlyFile *, void *);
	void ply_put_comment(PlyFile *, char *);
	void ply_put_obj_info(PlyFile *, char *);
	PlyFile *ply_read(FILE *);
	PlyFile *ply_open_for_reading(const char *);
	PlyProperty **ply_get_element_description(PlyFile *, char *, int*);
	char **ply_get_comments(PlyFile *, int *);
	char **ply_get_obj_info(PlyFile *, int *);
	void ply_close(PlyFile *);
	void get_ascii_item(const char *, int, int *, unsigned int *, double *);
	void get_binary_item(FILE *, int, int, int *, unsigned int *, double *);
	/* returns 1 if strings are equal, 0 if not */
	bool equal_strings(const char *, const char *);
	/* find an element in a plyfile's list */
	PlyElement *find_element(PlyFile *, const char *);
	/* find a property in an element's list */
	PlyProperty *find_property(PlyElement *, const char *, int *);
	/* write to a file the word describing a PLY file data type */
	void write_scalar_type(FILE *, int);
	/* read a line from a file and break it up into separate words */
	char **get_words(FILE *, int *, char **);
	/* write an item to a file */
	void write_binary_item(FILE *, int, int, unsigned int, double, int);
	void write_ascii_item(FILE *, int, unsigned int, double, int);
	/* add information to a PLY file descriptor */
	void add_element(PlyFile *, char **);
	void add_property(PlyFile *, char **);
	void add_comment(PlyFile *, char *);
	void add_obj_info(PlyFile *, char *);
	/* store a value into where a pointer and a type specify */
	void store_other_item(PlyOtherProperty*, int&, unsigned int&, double&);
	/* get a bunch of elements from a file */
	void ascii_get_element(PlyFile *, char *);
	void binary_get_element(PlyFile *, char *);
	/* byte ordering */
	void get_native_binary_type();
	void swap_bytes(char *, int);
	void check_types();
	int get_prop_type(char *type_name);

	/* free plyfile's memory */
	void free_ply_memory(PlyFile*, bool);
	/* malloc */
	void malloc_other_property(PlyOtherElem*, PlyOtherProperty*, bool);
	/* memory allocation */
	char *my_alloc(int, int, const char *);

private:

	bool vertexType[3]; // vertexType[0] -> ifHasColor, vertexType[1] -> ifHasNormal, vertexType[2] -> ifHasUV

	int native_binary_type;

	int types_checked = 0;

	typedef union
	{
		int  int_value;
		char byte_values[sizeof(int)];
	} endian_test_type;

	int ply_type_size[17] = {
		0,
		1,
		2,
		4,
		1,
		2,
		4,
		4,
		8,
		1,
		1,
		2,
		2,
		4,
		4,
		4,
		8
	};

	const char *type_names[17] = {
		"invalid",	  // not type 				   0
		"char",		  // character				   1
		"short",	  // short integer             2
		"int",        // integer                   4
		"uchar",      // unsigned character        1
		"ushort",     // unsigned short integer    2
		"uint",       // unsigned integer          4
		"float",      // single-precision float    4
		"double",     // double-precision float    8

		"int8",       // character                 1
		"uint8",      // unsigned character        1
		"int16",      // short integer             2
		"uint16",     // unsigned short integer    2
		"int32",      // integer                   4
		"uint32",     // unsigned integer          4
		"float32",    // single-precision float    4
		"float64",    // double-precision float    8
	};
};

#ifdef __cplusplus
}
#endif

/*************/
/*  Writing  */
/*************/

/******************************************************************************
Given a file pointer, get ready to write PLY data to the file.

Entry:
fp         - the given file pointer
nelems     - number of elements in object
elem_names - list of element names
file_type  - file type, either ascii or binary

Exit:
returns a pointer to a PlyFile, used to refer to this file, or NULL if error
******************************************************************************/

inline PlyFile * PlyFileReader::ply_write(
	FILE *fp,
	int nelems,
	const char **elem_names,
	int file_type
)
{
	PlyFile *plyfile;
	PlyElement *elem;
	/* check for NULL file pointer */
	if (fp == NULL)
		return (NULL);
	if (native_binary_type == -1)
		get_native_binary_type();
	if (!types_checked)
		check_types();
	/* create a record for this object */
	plyfile = (PlyFile *)myalloc(sizeof(PlyFile));
	if (file_type == PLY_FILE_TYPE::PLY_BINARY_NATIVE)
		plyfile->file_type = native_binary_type;
	else
		plyfile->file_type = file_type;
	plyfile->comments = NULL;
	plyfile->num_comments = 0;
	plyfile->obj_info = NULL;
	plyfile->num_obj_info = 0;
	plyfile->nelems = nelems;
	plyfile->version = 1.0;
	plyfile->fp = fp;
	plyfile->which_elem = NULL;
	plyfile->other_elems = NULL;
	/* tuck aside the names of the elements */
	plyfile->elems = (PlyElement **)myalloc(sizeof(PlyElement *) * nelems);
	for (int i = 0; i < nelems; i++) {
		elem = (PlyElement *)myalloc(sizeof(PlyElement));
		plyfile->elems[i] = elem;
		SAFE_STRCPY(elem->elemName, elem_names[i]);
		elem->eleNum = 0;
		elem->propNum = 0;
	}
	/* return pointer to the file descriptor */
	return (plyfile);
}


/******************************************************************************
Open a polygon file for writing.

Entry:
filename   - name of file to read from
nelems     - number of elements in object
elem_names - list of element names
file_type  - file type, either ascii or binary

Exit:
version - version number of PLY file
returns a file identifier, used to refer to this file, or NULL if error
******************************************************************************/

inline PlyFile * PlyFileReader::ply_open_for_writing(
	const char *filename,
	int nelems,
	const char **elem_names,
	int file_type,
	float *version
)
{
	PlyFile *plyfile;
	char *name;
	FILE *fp;
	/* tack on the extension .ply, if necessary */
	//name = (char *)myalloc(int(sizeof(char) * (strlen(filename)) + 6));
	//SAFE_STRCPY(name, filename);
	//if (strlen(name) < 4 ||
	//	strcmp(name + strlen(name) - 4, ".ply") != 0)
	//	strcat_s(name, int(sizeof(char) * (strlen(filename)) + 6), ".ply");
	/* open the file for writing */
	fp = fopen(filename, "wb");
	//free(name);
	if (fp == NULL) {
		return (NULL);
	}
	/* create the actual PlyFile structure */
	plyfile = ply_write(fp, nelems, elem_names, file_type);
	if (plyfile == NULL)
		return (NULL);
	/* say what PLY file version number we're writing */
	*version = plyfile->version;
	/* return pointer to the file descriptor */
	return (plyfile);
}

/******************************************************************************
Describe a property of an element.

Entry:
plyfile   - file identifier
elem_name - name of element that information is being specified about
prop      - the new property
******************************************************************************/

inline void PlyFileReader::ply_describe_property(
	PlyFile *plyfile,
	const char *elem_name,
	int propType
)
{
	PlyElement *elem;
	PlyProperty *elem_prop;
	/* look for appropriate element */
	elem = find_element(plyfile, elem_name);
	if (elem == NULL) {
		fprintf(stderr, "ply_describe_property: can't find element '%s'\n",
			elem_name);
		return;
	}
	/* create room for new property */
	if (elem->propNum == 0) {
		elem->propList = (PlyProperty **)myalloc(sizeof(PlyProperty *));
		elem->propNum = 1;
	}
	else {
		elem->propNum++;
		elem->propList = (PlyProperty **)
			realloc(elem->propList, sizeof(PlyProperty *) * elem->propNum);
	}
	/* copy the new property */
	elem_prop = (PlyProperty *)myalloc(sizeof(PlyProperty));
	if (propType == PLY_NAME_MARK::PLY_X)
	{
		SAFE_STRCPY(elem_prop->propName, "x");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_X;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_Y)
	{
		SAFE_STRCPY(elem_prop->propName, "y");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_Y;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_Z)
	{
		SAFE_STRCPY(elem_prop->propName, "z");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_Z;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_RED)
	{
		SAFE_STRCPY(elem_prop->propName, "red");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_RED;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_GREEN)
	{
		SAFE_STRCPY(elem_prop->propName, "green");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_GREEN;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_BLUE)
	{
		SAFE_STRCPY(elem_prop->propName, "blue");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_BLUE;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_NX)
	{
		SAFE_STRCPY(elem_prop->propName, "nx");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_NX;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_NY)
	{
		SAFE_STRCPY(elem_prop->propName, "ny");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_NY;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_NZ)
	{
		SAFE_STRCPY(elem_prop->propName, "nz");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_NZ;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_U)
	{
		SAFE_STRCPY(elem_prop->propName, "u");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_U;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_V)
	{
		SAFE_STRCPY(elem_prop->propName, "v");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_V;
		elem_prop->isList = false;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_FLOAT;
	}
	else if (propType == PLY_NAME_MARK::PLY_VERTEXS)
	{
		SAFE_STRCPY(elem_prop->propName, "vertex_indices");
		elem_prop->nameMark = PLY_NAME_MARK::PLY_VERTEXS;
		elem_prop->isList = true;
		elem_prop->countType = PLY_DATA_TYPE::PLY_UCHAR;
		elem_prop->externalType = PLY_DATA_TYPE::PLY_INT;
	}

	elem->propList[elem->propNum - 1] = elem_prop;
}

/******************************************************************************
State how many of a given element will be written.

Entry:
plyfile   - file identifier
elem_name - name of element that information is being specified about
nelems    - number of elements of this type to be written
******************************************************************************/

inline void PlyFileReader::ply_element_count(
	PlyFile *plyfile,
	const char *elem_name,
	int nelems
)
{
	PlyElement *elem;
	/* look for appropriate element */
	elem = find_element(plyfile, elem_name);
	if (elem == NULL) {
		fprintf(stderr, "ply_element_count: can't find element '%s'\n", elem_name);
		exit(-1);
	}
	elem->eleNum = nelems;
}

/******************************************************************************
Signal that we've described everything a PLY file's header and that the
header should be written to the file.

Entry:
plyfile - file identifier
******************************************************************************/

inline void PlyFileReader::ply_header_complete(PlyFile *plyfile)
{
	int i, j;
	FILE *fp = plyfile->fp;
	PlyElement *elem;
	PlyProperty *prop;
	fprintf(fp, "ply\n");
	switch (plyfile->file_type) {
	case PLY_FILE_TYPE::PLY_ASCII:
		fprintf(fp, "format ascii 1.0\n");
		break;
	case PLY_FILE_TYPE::PLY_BINARY_BE:
		fprintf(fp, "format binary_big_endian 1.0\n");
		break;
	case PLY_FILE_TYPE::PLY_BINARY_LE:
		fprintf(fp, "format binary_little_endian 1.0\n");
		break;
	default:
		fprintf(stderr, "ply_header_complete: bad file type = %d\n",
			plyfile->file_type);
		exit(-1);
	}
	/* write out the comments */
	for (i = 0; i < plyfile->num_comments; i++)
		fprintf(fp, "comment %s\n", plyfile->comments[i]);
	/* write out object information */
	for (i = 0; i < plyfile->num_obj_info; i++)
		fprintf(fp, "obj_info %s\n", plyfile->obj_info[i]);
	/* write out information about each element */
	for (i = 0; i < plyfile->nelems; i++) {
		elem = plyfile->elems[i];
		fprintf(fp, "element %s %d\n", elem->elemName, elem->eleNum);
		/* write out each property */
		for (j = 0; j < elem->propNum; j++) {
			prop = elem->propList[j];
			if (prop->isList) {
				fprintf(fp, "property list ");
				write_scalar_type(fp, prop->countType);
				fprintf(fp, " ");
				write_scalar_type(fp, prop->externalType);
				fprintf(fp, " %s\n", prop->propName);
			}
			else {
				fprintf(fp, "property ");
				write_scalar_type(fp, prop->externalType);
				fprintf(fp, " %s\n", prop->propName);
			}
		}
	}
	fprintf(fp, "end_header\n");
}

/******************************************************************************
Specify which elements are going to be written.  This should be called
before a call to the routine ply_put_element().

Entry:
plyfile   - file identifier
elem_name - name of element we're talking about
******************************************************************************/

inline void PlyFileReader::ply_put_element_setup(PlyFile *plyfile, const char *elem_name)
{
	PlyElement *elem;
	elem = find_element(plyfile, elem_name);
	if (elem == NULL) {
		fprintf(stderr, "ply_elements_setup: can't find element '%s'\n", elem_name);
		exit(-1);
	}
	plyfile->which_elem = elem;
}

/******************************************************************************
Specify a comment that will be written in the header.

Entry:
plyfile - file identifier
comment - the comment to be written
******************************************************************************/

inline void PlyFileReader::ply_put_comment(PlyFile *plyfile, char *comment)
{
	/* (re)allocate space for new comment */
	if (plyfile->num_comments == 0)
		plyfile->comments = (char **)myalloc(sizeof(char *));
	else
		plyfile->comments = (char **)realloc(plyfile->comments,
			sizeof(char *) * (plyfile->num_comments + 1));
	/* add comment to list */
	plyfile->comments[plyfile->num_comments] = _strdup(comment);
	plyfile->num_comments++;
}

/******************************************************************************
Specify a piece of object information (arbitrary text) that will be written
in the header.

Entry:
plyfile  - file identifier
obj_info - the text information to be written
******************************************************************************/

inline void PlyFileReader::ply_put_obj_info(PlyFile *plyfile, char *obj_info)
{
	/* (re)allocate space for new info */
	if (plyfile->num_obj_info == 0)
		plyfile->obj_info = (char **)myalloc(sizeof(char *));
	else
		plyfile->obj_info = (char **)realloc(plyfile->obj_info,
			sizeof(char *) * (plyfile->num_obj_info + 1));
	/* add info to list */
	plyfile->obj_info[plyfile->num_obj_info] = _strdup(obj_info);
	plyfile->num_obj_info++;
}

/*************/
/*  Reading  */
/*************/

/******************************************************************************
Given a file pointer, get ready to read PLY data from the file.

Entry:
fp - the given file pointer

Exit:
nelems     - number of elements in object
elem_names - list of element names
returns a pointer to a PlyFile, used to refer to this file, or NULL if error
******************************************************************************/

inline PlyFile *PlyFileReader::ply_read(FILE *fp)
{
	PlyFile *plyfile;
	int nwords;
	char **words;
	char *orig_line;
	/* check for NULL file pointer */
	if (fp == NULL)
		return (NULL);
	if (native_binary_type == -1)
		get_native_binary_type();
	if (!types_checked)
		check_types();
	/* create record for this object */
	plyfile = (PlyFile *)myalloc(sizeof(PlyFile));
	plyfile->elems = NULL;
	plyfile->nelems = 0;
	plyfile->comments = NULL;
	plyfile->num_comments = 0;
	plyfile->obj_info = NULL;
	plyfile->num_obj_info = 0;
	plyfile->fp = fp;
	plyfile->which_elem = NULL;
	PlyOtherData* currentOtherData = (PlyOtherData *)myalloc(sizeof(PlyOtherData));
	currentOtherData->elemList = NULL;
	currentOtherData->elemNum = 0;
	plyfile->other_elems = currentOtherData;
	/* read and parse the file's header */
	words = get_words(plyfile->fp, &nwords, &orig_line);
	if (!words || !equal_strings(words[0], "ply"))
	{
		if (words)
		{
			free(words);
			words = NULL;
		}
		return (NULL);
	}
	while (words) {
		/* parse words */
		if (equal_strings(words[0], "format")) {
			if (nwords != 3) {
				free(words);
				words = NULL;
				return (NULL);
			}
			if (equal_strings(words[1], "ascii"))
				plyfile->file_type = PLY_FILE_TYPE::PLY_ASCII;
			else if (equal_strings(words[1], "binary_big_endian"))
				plyfile->file_type = PLY_FILE_TYPE::PLY_BINARY_BE;
			else if (equal_strings(words[1], "binary_little_endian"))
				plyfile->file_type = PLY_FILE_TYPE::PLY_BINARY_LE;
			else {
				free(words);
				words = NULL;
				return (NULL);
			}
			plyfile->version = (float)atof(words[2]);
		}
		else if (equal_strings(words[0], "element"))
			add_element(plyfile, words);
		else if (equal_strings(words[0], "property"))
			add_property(plyfile, words);
		else if (equal_strings(words[0], "comment"))
			add_comment(plyfile, orig_line);
		else if (equal_strings(words[0], "obj_info"))
			add_obj_info(plyfile, orig_line);
		else if (equal_strings(words[0], "end_header")) {
			free(words);
			words = NULL;
			break;
		}
		/* free up words space */
		free(words);
		words = get_words(plyfile->fp, &nwords, &orig_line);
	}
	currentOtherData->fileType = plyfile->file_type;
	/* return a pointer to the file's information */
	return (plyfile);
}

/******************************************************************************
Open a polygon file for reading.

Entry:
filename - name of file to read from

Exit:
nelems     - number of elements in object
elem_names - list of element names
file_type  - file type, either ascii or binary
version    - version number of PLY file
returns a file identifier, used to refer to this file, or NULL if error
******************************************************************************/

inline PlyFile *PlyFileReader::ply_open_for_reading(
	const char *filename
	)
{
	FILE *fp;
	PlyFile *plyfile;
	char *name;
	/* tack on the extension .ply, if necessary */
	name = (char *)myalloc(int(sizeof(char) * (strlen(filename) + 5)));
	SAFE_STRCPY(name, filename);
	if (strlen(name) < 4 ||
		strcmp(name + strlen(name) - 4, ".ply") != 0)
		SAFE_STRCAT(name, ".ply");
	/* open the file for reading */
	fp = fopen(name, "rb");
	free(name);
	name = NULL;
	if (fp == NULL)
		return (NULL);
	/* create the PlyFile data structure */
	plyfile = ply_read(fp);
	/* return a pointer to the file's information */
	return (plyfile);
}

/******************************************************************************
Get information about a particular element.

Entry:
plyfile   - file identifier
elem_name - name of element to get information about

Exit:
nelems   - number of elements of this type in the file
nprops   - number of properties
returns a list of properties, or NULL if the file doesn't contain that elem
******************************************************************************/

inline PlyProperty **PlyFileReader::ply_get_element_description(
	PlyFile *plyfile,
	char *elem_name,
	int *nelems
)
{
	PlyElement *elem;
	//PlyProperty *prop;
	/* find information about the element */
	elem = find_element(plyfile, elem_name);
	if (elem == NULL)
		return (NULL);
	*nelems = elem->eleNum;
	/* return this duplicate property list */
	return (elem->propList);
}

/******************************************************************************
Extract the comments from the header information of a PLY file.

Entry:
plyfile - file identifier

Exit:
num_comments - number of comments returned
returns a pointer to a list of comments
******************************************************************************/

inline char **PlyFileReader::ply_get_comments(PlyFile *plyfile, int *num_comments)
{
	*num_comments = plyfile->num_comments;
	return (plyfile->comments);
}

/******************************************************************************
Extract the object information (arbitrary text) from the header information
of a PLY file.

Entry:
plyfile - file identifier

Exit:
num_obj_info - number of lines of text information returned
returns a pointer to a list of object info lines
******************************************************************************/

inline char **PlyFileReader::ply_get_obj_info(PlyFile *plyfile, int *num_obj_info)
{
	*num_obj_info = plyfile->num_obj_info;
	return (plyfile->obj_info);
}

/*******************/
/*  Miscellaneous  */
/*******************/

/******************************************************************************
Close a PLY file.

Entry:
plyfile - identifier of file to close
******************************************************************************/

inline void PlyFileReader::ply_close(PlyFile *plyfile)
{
	fclose(plyfile->fp);
	/* free up memory associated with the PLY file */
	free(plyfile);
	plyfile = NULL;
}

/******************************************************************************
Compare two strings.  Returns 1 if they are the same, 0 if not.
******************************************************************************/

inline bool PlyFileReader::equal_strings(const char *s1, const char *s2)
{
	while (*s1 && *s2)
		if (*s1++ != *s2++)
			return false;
	if (*s1 != *s2)
		return false;
	else
		return true;
}

/******************************************************************************
Find an element from the element list of a given PLY object.

Entry:
plyfile - file id for PLY file
element - name of element we're looking for

Exit:
returns the element, or NULL if not found
******************************************************************************/

inline PlyElement *PlyFileReader::find_element(PlyFile *plyfile, const char *element)
{
	for (int i = 0; i < plyfile->nelems; i++)
		if (equal_strings(element, plyfile->elems[i]->elemName))
			return (plyfile->elems[i]);
	return (NULL);
}

/******************************************************************************
Find a property in the list of properties of a given element.

Entry:
elem      - pointer to element in which we want to find the property
prop_name - name of property to find

Exit:
index - index to position in list
returns a pointer to the property, or NULL if not found
******************************************************************************/

inline PlyProperty *PlyFileReader::find_property(PlyElement *elem, const char *prop_name, int *index)
{
	for (int i = 0; i < elem->propNum; i++)
		if (equal_strings(prop_name, elem->propList[i]->propName)) {
			*index = i;
			return (elem->propList[i]);
		}
	*index = -1;
	return (NULL);
}

/******************************************************************************
Write to a file the word that represents a PLY data type.

Entry:
fp   - file pointer
code - code for type
******************************************************************************/

inline void PlyFileReader::write_scalar_type(FILE *fp, int code)
{
	/* make sure this is a valid code */
	if (code <= PLY_DATA_TYPE::PLY_START_TYPE || code >= PLY_DATA_TYPE::PLY_END_TYPE) {
		fprintf(stderr, "write_scalar_type: bad data code = %d\n", code);
		exit(-1);
	}
	/* write the code to a file */
	fprintf(fp, "%s", type_names[code]);
}

/******************************************************************************
Get a text line from a file and break it up into words.

IMPORTANT: The calling routine call "free" on the returned pointer once
finished with it.

Entry:
fp - file to read from

Exit:
nwords    - number of words returned
orig_line - the original line of characters
returns a list of words from the line, or NULL if end-of-file
******************************************************************************/

inline char **PlyFileReader::get_words(FILE *fp, int *nwords, char **orig_line)
{
#define BIG_STRING 4096
	static char str[BIG_STRING];
	static char str_copy[BIG_STRING];
	char **words;
	int max_words = 10;
	int num_words = 0;
	char *ptr, *ptr2;
	char *result;
	words = (char **)myalloc(sizeof(char *) * max_words);
	/* read in a line */
	result = fgets(str, BIG_STRING, fp);
	if (result == NULL) {
		free(words);
		words = NULL;
		*nwords = 0;
		*orig_line = NULL;
		return (NULL);
	}
	/* convert line-feed and tabs into spaces */
	/* (this guarentees that there will be a space before the */
	/*  null character at the end of the string) */
	str[BIG_STRING - 2] = ' ';
	str[BIG_STRING - 1] = '\0';
	for (ptr = str, ptr2 = str_copy; *ptr != '\0'; ptr++, ptr2++) {
		*ptr2 = *ptr;
		// Added line here to manage carriage returns
		if (*ptr == '\t' || *ptr == '\r') {
			*ptr = ' ';
			*ptr2 = ' ';
		}
		else if (*ptr == '\n') {
			*ptr = ' ';
			*ptr2 = '\0';
			break;
		}
	}
	/* find the words in the line */
	ptr = str;
	while (*ptr != '\0') {
		/* jump over leading spaces */
		while (*ptr == ' ')
			ptr++;
		/* break if we reach the end */
		if (*ptr == '\0')
			break;
		/* save pointer to beginning of word */
		if (num_words >= max_words) {
			max_words += 10;
			words = (char **)realloc(words, sizeof(char *) * max_words);
		}
		words[num_words++] = ptr;
		/* jump over non-spaces */
		while (*ptr != ' ')
			ptr++;
		/* place a null character here to mark the end of the word */
		*ptr++ = '\0';
	}
	/* return the list of words */
	*nwords = num_words;
	*orig_line = str_copy;
	return (words);
}

/******************************************************************************
Write out an item to a file as raw binary bytes.

Entry:
fp         - file to write to
int_val    - integer version of item
uint_val   - unsigned integer version of item
double_val - double-precision float version of item
type       - data type to write out
******************************************************************************/

inline void PlyFileReader::write_binary_item(
	FILE *fp,
	int file_type,
	int int_val,
	unsigned int uint_val,
	double double_val,
	int type
)
{
	unsigned char uchar_val;
	char char_val;
	unsigned short ushort_val;
	short short_val;
	float float_val;
	void  *value;

	switch (type) {
	case PLY_DATA_TYPE::PLY_CHAR:
	case PLY_DATA_TYPE::PLY_INT_8:
		char_val = char(int_val);
		value = &char_val;
		break;
	case PLY_DATA_TYPE::PLY_SHORT:
	case PLY_DATA_TYPE::PLY_INT_16:
		short_val = short(int_val);
		value = &short_val;
		break;
	case PLY_DATA_TYPE::PLY_INT:
	case PLY_DATA_TYPE::PLY_INT_32:
		value = &int_val;
		break;
	case PLY_DATA_TYPE::PLY_UCHAR:
	case PLY_DATA_TYPE::PLY_UINT_8:
		uchar_val = (unsigned char)(uint_val);
		value = &uchar_val;
		break;
	case PLY_DATA_TYPE::PLY_USHORT:
	case PLY_DATA_TYPE::PLY_UINT_16:
		ushort_val = (unsigned short)(uint_val);
		value = &ushort_val;
		break;
	case PLY_DATA_TYPE::PLY_UINT:
	case PLY_DATA_TYPE::PLY_UINT_32:
		value = &uint_val;
		break;
	case PLY_DATA_TYPE::PLY_FLOAT:
	case PLY_DATA_TYPE::PLY_FLOAT_32:
		float_val = (float)double_val;
		value = &float_val;
		break;
	case PLY_DATA_TYPE::PLY_DOUBLE:
	case PLY_DATA_TYPE::PLY_FLOAT_64:
		value = &double_val;
		break;
	default:
		fprintf(stderr, "write_binary_item: bad type = %d\n", type);
		exit(-1);
	}
	if ((file_type != native_binary_type) && (ply_type_size[type] > 1))
		swap_bytes((char *)value, ply_type_size[type]);

	if (fwrite(value, ply_type_size[type], 1, fp) != 1)
	{
		fprintf(stderr, "PLY ERROR: fwrite() failed -- aborting.\n");
		exit(1);
	}
}

/******************************************************************************
Write out an item to a file as ascii characters.

Entry:
fp         - file to write to
int_val    - integer version of item
uint_val   - unsigned integer version of item
double_val - double-precision float version of item
type       - data type to write out
******************************************************************************/

inline void PlyFileReader::write_ascii_item(
	FILE *fp,
	int int_val,
	unsigned int uint_val,
	double double_val,
	int type
)
{
	switch (type) {
	case PLY_DATA_TYPE::PLY_CHAR:
	case PLY_DATA_TYPE::PLY_INT_8:
	case PLY_DATA_TYPE::PLY_SHORT:
	case PLY_DATA_TYPE::PLY_INT_16:
	case PLY_DATA_TYPE::PLY_INT:
	case PLY_DATA_TYPE::PLY_INT_32:
		if (fprintf(fp, "%d ", int_val) <= 0)
		{
			fprintf(stderr, "PLY ERROR: fprintf() failed -- aborting.\n");
			exit(1);
		}
		break;
	case PLY_DATA_TYPE::PLY_UCHAR:
	case PLY_DATA_TYPE::PLY_UINT_8:
	case PLY_DATA_TYPE::PLY_USHORT:
	case PLY_DATA_TYPE::PLY_UINT_16:
	case PLY_DATA_TYPE::PLY_UINT:
	case PLY_DATA_TYPE::PLY_UINT_32:
		if (fprintf(fp, "%u ", uint_val) <= 0)
		{
			fprintf(stderr, "PLY ERROR: fprintf() failed -- aborting.\n");
			exit(1);
		}
		break;
	case PLY_DATA_TYPE::PLY_FLOAT:
	case PLY_DATA_TYPE::PLY_FLOAT_32:
	case PLY_DATA_TYPE::PLY_DOUBLE:
	case PLY_DATA_TYPE::PLY_FLOAT_64:
		if (fprintf(fp, "%g ", double_val) <= 0)
		{
			fprintf(stderr, "PLY ERROR: fprintf() failed -- aborting.\n");
			exit(1);
		}
		break;
	default:
		fprintf(stderr, "write_ascii_item: bad type = %d\n", type);
		exit(-1);
	}
}

/******************************************************************************
Add an element to a PLY file descriptor.

Entry:
plyfile - PLY file descriptor
words   - list of words describing the element
nwords  - number of words in the list
******************************************************************************/

inline void PlyFileReader::add_element(PlyFile *plyfile, char **words)
{
	PlyOtherData* currentOtherData = plyfile->other_elems;
	PlyElement *elem;
	PlyOtherElem * otherElem;
	/* create the new element */
	elem = (PlyElement*)myalloc(sizeof(PlyElement));
	SAFE_STRCPY(elem->elemName, words[1]);
	elem->eleNum = atoi(words[2]);
	elem->propNum = 0;
	elem->propList = NULL;
	/* create the new other element*/
	otherElem = (PlyOtherElem*)myalloc(sizeof(PlyOtherElem));
	SAFE_STRCPY(otherElem->elemName, elem->elemName);
	otherElem->elemCount = elem->eleNum;
	otherElem->propNum = 0;
	otherElem->propList = NULL;
	/* make room for new element in the object's list of elements */
	if (plyfile->nelems == 0)
		plyfile->elems = (PlyElement **)myalloc(sizeof(PlyElement *));
	else
		plyfile->elems = (PlyElement **)realloc(plyfile->elems,
			sizeof(PlyElement *) * (plyfile->nelems + 1));
	/* make room for new other element in the object's list of elements */
	if (currentOtherData->elemNum == 0)
		currentOtherData->elemList = (PlyOtherElem**)myalloc(sizeof(PlyOtherElem*));
	else
		currentOtherData->elemList = (PlyOtherElem**)realloc(currentOtherData->elemList,
			sizeof(PlyOtherElem*) * (currentOtherData->elemNum + 1));
	/* add the new element to the object's list */
	plyfile->elems[plyfile->nelems] = elem;
	plyfile->nelems++;
	/* add the new other element to the object's list */
	currentOtherData->elemList[currentOtherData->elemNum] = otherElem;
	currentOtherData->elemNum++;
}

/******************************************************************************
Return the type of a property, given the name of the property.

Entry:
name - name of property type

Exit:
returns integer code for property, or 0 if not found
******************************************************************************/

inline int PlyFileReader::get_prop_type(char *type_name)
{
	int i;

	for (i = PLY_DATA_TYPE::PLY_START_TYPE + 1; i < PLY_DATA_TYPE::PLY_END_TYPE; i++)
		if (equal_strings(type_name, type_names[i]))
			return (i);

	/* if we get here, we didn't find the type */
	return (0);
}

/******************************************************************************
Add a property to a PLY file descriptor.

Entry:
plyfile - PLY file descriptor
words   - list of words describing the property
nwords  - number of words in the list
******************************************************************************/

inline void PlyFileReader::add_property(PlyFile *plyfile, char **words)
{
	PlyOtherProperty* otherProp = NULL;
	PlyProperty *prop = NULL;

	PlyElement *elem = plyfile->elems[plyfile->nelems - 1];
	prop = (PlyProperty *)myalloc(sizeof(PlyProperty));
	/* create the new property */
	if (equal_strings(words[1], "list")) {       /* is a list */
		prop->countType = get_prop_type(words[2]);
		prop->externalType = get_prop_type(words[3]);
		SAFE_STRCPY(prop->propName, words[4]);
		prop->isList = true;
		if (equal_strings("face", elem->elemName))
		{
			if (equal_strings(words[4], "vertex_indices"))
			{
				prop->nameMark = PLY_NAME_MARK::PLY_VERTEXS;
			}
			else
			{
				prop->nameMark = PLY_NAME_MARK::PLY_OTHER;
				otherProp = (PlyOtherProperty*)myalloc(sizeof(PlyOtherProperty));
				otherProp->countType = get_prop_type(words[2]);
				otherProp->externalType = get_prop_type(words[3]);
				SAFE_STRCPY(otherProp->propName, words[4]);
				otherProp->isList = true;
				otherProp->ifMalloc = false;
				otherProp->listCount = 0;
				otherProp->offset = 0;
				otherProp->intValues = NULL;
				otherProp->uintValues = NULL;
				otherProp->doubleValues = NULL;
			}
		}
		else
		{
			prop->nameMark = PLY_NAME_MARK::PLY_OTHER;
			otherProp = (PlyOtherProperty*)myalloc(sizeof(PlyOtherProperty));
			otherProp->countType = get_prop_type(words[2]);
			otherProp->externalType = get_prop_type(words[3]);
			SAFE_STRCPY(otherProp->propName, words[4]);
			otherProp->isList = true;
			otherProp->ifMalloc = false;
			otherProp->listCount = 0;
			otherProp->offset = 0;
			otherProp->intValues = NULL;
			otherProp->uintValues = NULL;
			otherProp->doubleValues = NULL;
		}
	}
	else {                                        /* not a list */
		prop->externalType = get_prop_type(words[1]);
		SAFE_STRCPY(prop->propName, words[2]);
		prop->isList = false;
		if (equal_strings("vertex", elem->elemName))
		{
			if (equal_strings("x", words[2]))
				prop->nameMark = PLY_NAME_MARK::PLY_X;
			else if (equal_strings("y", words[2]))
				prop->nameMark = PLY_NAME_MARK::PLY_Y;
			else if (equal_strings("z", words[2]))
				prop->nameMark = PLY_NAME_MARK::PLY_Z;
			else if (equal_strings("red", words[2]) && vertexType[0])
				prop->nameMark = PLY_NAME_MARK::PLY_RED;
			else if (equal_strings("green", words[2]) && vertexType[0])
				prop->nameMark = PLY_NAME_MARK::PLY_GREEN;
			else if (equal_strings("blue", words[2]) && vertexType[0])
				prop->nameMark = PLY_NAME_MARK::PLY_BLUE;
			else if (equal_strings("nx", words[2]) && vertexType[1])
				prop->nameMark = PLY_NAME_MARK::PLY_NX;
			else if (equal_strings("ny", words[2]) && vertexType[1])
				prop->nameMark = PLY_NAME_MARK::PLY_NY;
			else if (equal_strings("nz", words[2]) && vertexType[1])
				prop->nameMark = PLY_NAME_MARK::PLY_NZ;
			else if (equal_strings("u", words[2]) && vertexType[2])
				prop->nameMark = PLY_NAME_MARK::PLY_U;
			else if (equal_strings("v", words[2]) && vertexType[2])
				prop->nameMark = PLY_NAME_MARK::PLY_V;
			else {
				prop->nameMark = PLY_NAME_MARK::PLY_OTHER;
				otherProp = (PlyOtherProperty*)myalloc(sizeof(PlyOtherProperty));
				otherProp->externalType = get_prop_type(words[1]);
				SAFE_STRCPY(otherProp->propName, words[2]);
				otherProp->isList = false;
				otherProp->ifMalloc = false;
				otherProp->listCount = 0;
				otherProp->offset = 0;
				otherProp->intValues = NULL;
				otherProp->uintValues = NULL;
				otherProp->doubleValues = NULL;
			}
		}
		else {
			prop->nameMark = PLY_NAME_MARK::PLY_OTHER;
			otherProp = (PlyOtherProperty*)myalloc(sizeof(PlyOtherProperty));
			otherProp->externalType = get_prop_type(words[1]);
			SAFE_STRCPY(otherProp->propName, words[2]);
			otherProp->isList = false;
			otherProp->ifMalloc = false;
			otherProp->listCount = 0;
			otherProp->offset = 0;
			otherProp->intValues = NULL;
			otherProp->uintValues = NULL;
			otherProp->doubleValues = NULL;
		}
	}
	/* add this property to the list of properties of the current element */
	if (elem->propNum == 0)
		elem->propList = (PlyProperty **)myalloc(sizeof(PlyProperty *));
	else
		elem->propList = (PlyProperty **)realloc(elem->propList,
			sizeof(PlyProperty *) * (elem->propNum + 1));
	elem->propList[elem->propNum] = prop;
	elem->propNum++;
	/* if otherProp exist add this property to the list of properties of the current other element */
	if (otherProp != NULL)
	{
		PlyOtherElem* otherElem = plyfile->other_elems->elemList[plyfile->other_elems->elemNum - 1];
		if (otherElem->propNum == 0)
			otherElem->propList = (PlyOtherProperty**)myalloc(sizeof(PlyOtherProperty*));
		else
			otherElem->propList = (PlyOtherProperty**)realloc(otherElem->propList,
				sizeof(PlyOtherProperty*) * (otherElem->propNum + 1));
		otherElem->propList[otherElem->propNum] = otherProp;
		otherElem->propNum++;
	}
}

/******************************************************************************
Add a comment to a PLY file descriptor.

Entry:
plyfile - PLY file descriptor
line    - line containing comment
******************************************************************************/

inline void PlyFileReader::add_comment(PlyFile *plyfile, char *line)
{
	/* skip over "comment" and leading spaces and tabs */
	int i = 7;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	ply_put_comment(plyfile, &line[i]);
}

/******************************************************************************
Add a some object information to a PLY file descriptor.

Entry:
plyfile - PLY file descriptor
line    - line containing text info
******************************************************************************/

inline void PlyFileReader::add_obj_info(PlyFile *plyfile, char *line)
{
	/* skip over "obj_info" and leading spaces and tabs */
	int i = 8;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	ply_put_obj_info(plyfile, &line[i]);
}


/******************************************************************************
Store a value into a place being pointed to, guided by a data type.

Entry:
item       - place to store value
type       - data type
int_val    - integer version of value
uint_val   - unsigned integer version of value
double_val - double version of value

Exit:
item - pointer to stored value
******************************************************************************/

inline void PlyFileReader::store_other_item(
	PlyOtherProperty* currentOtherProp,
	int& int_val,
	unsigned int& uint_val,
	double& double_val
)
{
	switch (currentOtherProp->externalType) {
	case PLY_DATA_TYPE::PLY_CHAR:
	case PLY_DATA_TYPE::PLY_INT_8:
	case PLY_DATA_TYPE::PLY_UCHAR:
	case PLY_DATA_TYPE::PLY_UINT_8:
	case PLY_DATA_TYPE::PLY_SHORT:
	case PLY_DATA_TYPE::PLY_INT_16:
	case PLY_DATA_TYPE::PLY_USHORT:
	case PLY_DATA_TYPE::PLY_UINT_16:
	case PLY_DATA_TYPE::PLY_INT:
	case PLY_DATA_TYPE::PLY_INT_32:
		currentOtherProp->intValues[currentOtherProp->offset] = int_val;
		currentOtherProp->offset++;
		break;
	case PLY_DATA_TYPE::PLY_UINT:
	case PLY_DATA_TYPE::PLY_UINT_32:
		currentOtherProp->uintValues[currentOtherProp->offset] = uint_val;
		currentOtherProp->offset++;
		break;
	case PLY_DATA_TYPE::PLY_FLOAT:
	case PLY_DATA_TYPE::PLY_FLOAT_32:
	case PLY_DATA_TYPE::PLY_DOUBLE:
	case PLY_DATA_TYPE::PLY_FLOAT_64:
		currentOtherProp->doubleValues[currentOtherProp->offset] = double_val;
		currentOtherProp->offset++;
		break;
	default:
		fprintf(stderr, "store_item: bad type = %d\n", currentOtherProp->externalType);
		exit(-1);
	}
}

/******************************************************************************
Extract the value of an item from an ascii word, and place the result
into an integer, an unsigned integer and a double.

Entry:
word - word to extract value from
type - data type supposedly in the word

Exit:
int_val    - integer value
uint_val   - unsigned integer value
double_val - double-precision floating point value
******************************************************************************/

inline void PlyFileReader::get_ascii_item(
	const char *word,
	int type,
	int *int_val,
	unsigned int *uint_val,
	double *double_val
)
{
	switch (type) {
	case PLY_DATA_TYPE::PLY_CHAR:
	case PLY_DATA_TYPE::PLY_INT_8:
	case PLY_DATA_TYPE::PLY_UCHAR:
	case PLY_DATA_TYPE::PLY_UINT_8:
	case PLY_DATA_TYPE::PLY_SHORT:
	case PLY_DATA_TYPE::PLY_INT_16:
	case PLY_DATA_TYPE::PLY_USHORT:
	case PLY_DATA_TYPE::PLY_UINT_16:
	case PLY_DATA_TYPE::PLY_INT:
	case PLY_DATA_TYPE::PLY_INT_32:
		*int_val = atoi(word);
		*uint_val = (unsigned int)*int_val;
		*double_val = (double)*int_val;
		break;
	case PLY_DATA_TYPE::PLY_UINT:
	case PLY_DATA_TYPE::PLY_UINT_32:
		*uint_val = strtol(word, (char **)NULL, 10);
		*int_val = (int)*uint_val;
		*double_val = (double)*uint_val;
		break;
	case PLY_DATA_TYPE::PLY_FLOAT:
	case PLY_DATA_TYPE::PLY_FLOAT_32:
	case PLY_DATA_TYPE::PLY_DOUBLE:
	case PLY_DATA_TYPE::PLY_FLOAT_64:
		*double_val = atof(word);
		*int_val = (int)*double_val;
		*uint_val = (unsigned int)*double_val;
		break;
	default:
		fprintf(stderr, "get_ascii_item: bad type = %d\n", type);
		exit(-1);
	}
}

/******************************************************************************
Get the value of an item from a binary file, and place the result
into an integer, an unsigned integer and a double.

Entry:
fp   - file to get item from
type - data type supposedly in the word

Exit:
int_val    - integer value
uint_val   - unsigned integer value
double_val - double-precision floating point value
******************************************************************************/

inline void PlyFileReader::get_binary_item(
	FILE *fp,
	int file_type,
	int type,
	int *int_val,
	unsigned int *uint_val,
	double *double_val
)
{
	char c[8];
	void *ptr;
	ptr = (void *)c;
	if (fread(ptr, ply_type_size[type], 1, fp) != 1)
	{
		fprintf(stderr, "PLY ERROR: fread() failed -- aborting.\n");
		exit(1);
	}
	if ((file_type != native_binary_type) && (ply_type_size[type] > 1))
		swap_bytes((char *)ptr, ply_type_size[type]);
	switch (type) {
	case PLY_DATA_TYPE::PLY_CHAR:
	case PLY_DATA_TYPE::PLY_INT_8:
		*int_val = *((char *)ptr);
		*uint_val = *int_val;
		*double_val = *int_val;
		break;
	case PLY_DATA_TYPE::PLY_UCHAR:
	case PLY_DATA_TYPE::PLY_UINT_8:
		*uint_val = *((unsigned char *)ptr);
		*int_val = *uint_val;
		*double_val = *uint_val;
		break;
	case PLY_DATA_TYPE::PLY_SHORT:
	case PLY_DATA_TYPE::PLY_INT_16:
		*int_val = *((short int *)ptr);
		*uint_val = *int_val;
		*double_val = *int_val;
		break;
	case PLY_DATA_TYPE::PLY_USHORT:
	case PLY_DATA_TYPE::PLY_UINT_16:
		*uint_val = *((unsigned short int *) ptr);
		*int_val = *uint_val;
		*double_val = *uint_val;
		break;
	case PLY_DATA_TYPE::PLY_INT:
	case PLY_DATA_TYPE::PLY_INT_32:
		*int_val = *((int *)ptr);
		*uint_val = *int_val;
		*double_val = *int_val;
		break;
	case PLY_DATA_TYPE::PLY_UINT:
	case PLY_DATA_TYPE::PLY_UINT_32:
		*uint_val = *((unsigned int *)ptr);
		*int_val = *uint_val;
		*double_val = *uint_val;
		break;
	case PLY_DATA_TYPE::PLY_FLOAT:
	case PLY_DATA_TYPE::PLY_FLOAT_32:
		*double_val = *((float *)ptr);
		*int_val = (int)*double_val;
		*uint_val = (unsigned int)*double_val;
		break;
	case PLY_DATA_TYPE::PLY_DOUBLE:
	case PLY_DATA_TYPE::PLY_FLOAT_64:
		*double_val = *((double *)ptr);
		*int_val = (int)*double_val;
		*uint_val = (unsigned int)*double_val;
		break;
	default:
		fprintf(stderr, "get_binary_item: bad type = %d\n", type);
		exit(-1);
	}
}

/******************************************************************************
Allocate some memory.

Entry:
size  - amount of memory requested (in bytes)
lnum  - line number from which memory was requested
fname - file name from which memory was requested
******************************************************************************/

inline char *PlyFileReader::my_alloc(int size, int lnum, const char *fname)
{
	char *ptr;
	ptr = (char *)malloc(size);
	if (ptr == 0) {
		fprintf(stderr, "Memory allocation bombed on line %d in %s\n", lnum, fname);
	}
	return (ptr);
}

/******************************************************************************
Find out if this machine is big endian or little endian

Exit:
set global variable, native_binary_type =
either PLY_BINARY_BE or PLY_BINARY_LE

******************************************************************************/

inline void PlyFileReader::get_native_binary_type()
{
	endian_test_type test;
	test.int_value = 0;
	test.int_value = 1;
	if (test.byte_values[0] == 1)
		native_binary_type = PLY_FILE_TYPE::PLY_BINARY_LE;
	else if (test.byte_values[sizeof(int) - 1] == 1)
		native_binary_type = PLY_FILE_TYPE::PLY_BINARY_BE;
	else
	{
		fprintf(stderr, "ply: Couldn't determine machine endianness.\n");
		fprintf(stderr, "ply: Exiting...\n");
		exit(1);
	}
}

/******************************************************************************
Reverse the order in an array of bytes.  This is the conversion from big
endian to little endian and vice versa

Entry:
bytes     - array of bytes to reverse (in place)
num_bytes - number of bytes in array
******************************************************************************/

inline void PlyFileReader::swap_bytes(char *bytes, int num_bytes)
{
	char temp;
	for (int i = 0; i < num_bytes / 2; i++)
	{
		temp = bytes[i];
		bytes[i] = bytes[(num_bytes - 1) - i];
		bytes[(num_bytes - 1) - i] = temp;
	}
}

/******************************************************************************
Verify that all the native types are the sizes we need

******************************************************************************/

inline void PlyFileReader::check_types()
{
	if ((ply_type_size[PLY_DATA_TYPE::PLY_CHAR] != sizeof(char)) ||
		(ply_type_size[PLY_DATA_TYPE::PLY_SHORT] != sizeof(short)) ||
		(ply_type_size[PLY_DATA_TYPE::PLY_INT] != sizeof(int)) ||
		(ply_type_size[PLY_DATA_TYPE::PLY_UCHAR] != sizeof(unsigned char)) ||
		(ply_type_size[PLY_DATA_TYPE::PLY_USHORT] != sizeof(unsigned short)) ||
		(ply_type_size[PLY_DATA_TYPE::PLY_UINT] != sizeof(unsigned int)) ||
		(ply_type_size[PLY_DATA_TYPE::PLY_FLOAT] != sizeof(float)) ||
		(ply_type_size[PLY_DATA_TYPE::PLY_DOUBLE] != sizeof(double)))
	{
		fprintf(stderr, "ply: Type sizes do not match built-in types\n");
		fprintf(stderr, "ply: Exiting...\n");
		exit(1);
	}
	types_checked = 1;
}

/* malloc */
inline void PlyFileReader::malloc_other_property(PlyOtherElem* currentOtherElem, PlyOtherProperty* currentOtherProp, bool ifList)
{
	switch (currentOtherProp->externalType)
	{
	case PLY_DATA_TYPE::PLY_CHAR:
	case PLY_DATA_TYPE::PLY_INT_8:
	case PLY_DATA_TYPE::PLY_UCHAR:
	case PLY_DATA_TYPE::PLY_UINT_8:
	case PLY_DATA_TYPE::PLY_SHORT:
	case PLY_DATA_TYPE::PLY_INT_16:
	case PLY_DATA_TYPE::PLY_USHORT:
	case PLY_DATA_TYPE::PLY_UINT_16:
	case PLY_DATA_TYPE::PLY_INT:
	case PLY_DATA_TYPE::PLY_INT_32:
		if(!ifList)
			currentOtherProp->intValues = (int*)myalloc(sizeof(int) * currentOtherElem->elemCount);
		else
			currentOtherProp->intValues = (int*)myalloc(sizeof(int) * currentOtherElem->elemCount * currentOtherProp->listCount);
		currentOtherProp->ifMalloc = true;
		break;
	case PLY_DATA_TYPE::PLY_UINT:
	case PLY_DATA_TYPE::PLY_UINT_32:
		if (!ifList)
			currentOtherProp->uintValues = (unsigned int*)myalloc(sizeof(unsigned int) * currentOtherElem->elemCount);
		else
			currentOtherProp->uintValues = (unsigned int*)myalloc(sizeof(unsigned int) * currentOtherElem->elemCount * currentOtherProp->listCount);
		currentOtherProp->ifMalloc = true;
		break;
	case PLY_DATA_TYPE::PLY_FLOAT:
	case PLY_DATA_TYPE::PLY_FLOAT_32:
	case PLY_DATA_TYPE::PLY_DOUBLE:
	case PLY_DATA_TYPE::PLY_FLOAT_64:
		if (!ifList)
			currentOtherProp->doubleValues = (double*)myalloc(sizeof(double) * currentOtherElem->elemCount);
		else
			currentOtherProp->doubleValues = (double*)myalloc(sizeof(double) * currentOtherElem->elemCount * currentOtherProp->listCount);
		currentOtherProp->ifMalloc = true;
		break;
	default:
		break;
	}
}

/* free plyfile's memory */
inline void PlyFileReader::free_ply_memory(PlyFile* plyFile, bool ifFreeOtherData)
{
	if (ifFreeOtherData)
	{
		PlyOtherData* currentOtherData = plyFile->other_elems;
		int numOtherElements = currentOtherData->elemNum;
		for (int j = 0; j < numOtherElements; j++)
		{
			PlyOtherElem* currentOtherElem = currentOtherData->elemList[j];
			for (int k = 0; k < currentOtherElem->propNum; k++)
			{
				PlyOtherProperty* currentOtherProp = currentOtherElem->propList[j];
				if (currentOtherProp->doubleValues != NULL)
				{
					free(currentOtherProp->doubleValues);
					currentOtherProp->doubleValues = NULL;
				}
				if (currentOtherProp->intValues != NULL)
				{
					free(currentOtherProp->intValues);
					currentOtherProp->intValues = NULL;
				}
				if (currentOtherProp->uintValues != NULL)
				{
					free(currentOtherProp->uintValues);
					currentOtherProp->uintValues = NULL;
				}
				free(currentOtherElem->propList[k]);
				currentOtherElem->propList[k] = NULL;
			}
			free(currentOtherElem->propList);
			currentOtherElem->propList = NULL;
		}
		for (int j = 0; j < numOtherElements; j++)
		{
			free(currentOtherData->elemList[j]);
			currentOtherData->elemList[j] = NULL;
		}
		free(currentOtherData->elemList);
		currentOtherData->elemList = NULL;
		free(currentOtherData);
		currentOtherData = NULL;
	}
	int numElements = plyFile->nelems;
	for (int j = 0; j < numElements; j++)
	{
		PlyElement* currentEle = plyFile->elems[j];
		for (int k = 0; k < currentEle->propNum; k++)
		{
			free(currentEle->propList[k]);
			currentEle->propList[k] = NULL;
		}
		if (currentEle->propList && currentEle->propNum)
		{
			free(currentEle->propList);
			currentEle->propList = NULL;
		}
	}
	for (int j = 0; j < numElements; j++)
	{
		free(plyFile->elems[j]);
		plyFile->elems[j] = NULL;
	}
	free(plyFile->elems);
	plyFile->elems = NULL;
	for (int j = 0; j < plyFile->num_comments; j++)
	{
		free(plyFile->comments[j]);
		plyFile->comments[j] = NULL;
	}
	free(plyFile->comments);
	plyFile->comments = NULL;
	for (int j = 0; j < plyFile->num_obj_info; j++)
	{
		free(plyFile->obj_info[j]);
		plyFile->obj_info[j] = NULL;
	}
	free(plyFile->obj_info);
	plyFile->obj_info = NULL;
	fclose(plyFile->fp);
	/* free up memory associated with the PLY file */
	free(plyFile);
	plyFile = NULL;
}



class MyPlyOtherData
{
public:
	MyPlyOtherData()
	{
		mOtherData = NULL;
	};
	MyPlyOtherData(PlyOtherData* otherData)
	{
		mOtherData = otherData;
	};
	~MyPlyOtherData()
	{
		for (int i = 0; i < mOtherData->elemNum; i++)
		{
			PlyOtherElem* currentOtherElem = mOtherData->elemList[i];
			for (int j = 0; j < currentOtherElem->propNum; j++)
			{
				PlyOtherProperty* currentOtherProp = currentOtherElem->propList[j];
				if (currentOtherProp->doubleValues != NULL)
				{
					free(currentOtherProp->doubleValues);
					currentOtherProp->doubleValues = NULL;
				}
				if (currentOtherProp->intValues != NULL)
				{
					free(currentOtherProp->intValues);
					currentOtherProp->intValues = NULL;
				}
				if (currentOtherProp->uintValues != NULL)
				{
					free(currentOtherProp->uintValues);
					currentOtherProp->uintValues = NULL;
				}
				free(currentOtherElem->propList[j]);
				currentOtherElem->propList[j] = NULL;
			}
			free(currentOtherElem->propList);
			currentOtherElem->propList = NULL;
		}
		for (int i = 0; i < mOtherData->elemNum; i++)
		{
			free(mOtherData->elemList[i]);
			mOtherData->elemList[i] = NULL;
		}
		free(mOtherData->elemList);
		mOtherData->elemList = NULL;
		free(mOtherData);
		mOtherData = NULL;
	};
	void SetPlyOtherData(PlyOtherData* otherData)
	{
		mOtherData = otherData;
	}
	void ShowData()
	{
		int elemNum = mOtherData->elemNum;
		for (int i = 0; i < elemNum; i++)
		{
			PlyOtherElem* currentOtherEle = mOtherData->elemList[i];
			std::cout << "----" << currentOtherEle->elemName << "----" << std::endl;
			int propNum = currentOtherEle->propNum;
			for (int j = 0; j < propNum; j++)
			{
				PlyOtherProperty* currentOtherProp = currentOtherEle->propList[j];
				std::cout << "----" << currentOtherProp->propName << "----" << std::endl;
				switch (currentOtherProp->externalType)
				{
				case PLY_DATA_TYPE::PLY_CHAR:
				case PLY_DATA_TYPE::PLY_INT_8:
				case PLY_DATA_TYPE::PLY_UCHAR:
				case PLY_DATA_TYPE::PLY_UINT_8:
				case PLY_DATA_TYPE::PLY_SHORT:
				case PLY_DATA_TYPE::PLY_INT_16:
				case PLY_DATA_TYPE::PLY_USHORT:
				case PLY_DATA_TYPE::PLY_UINT_16:
				case PLY_DATA_TYPE::PLY_INT:
				case PLY_DATA_TYPE::PLY_INT_32:
					if (!currentOtherProp->isList)
					{
						int dataNum = currentOtherEle->elemCount;
						for (int k = 0; k < dataNum; k++)
						{
							std::cout << currentOtherProp->intValues[k] << std::endl;
						}
					}
					else
					{
						int dataNum = currentOtherEle->elemCount * currentOtherProp->listCount;
						for (int k = 0; k < dataNum; k++)
						{
							std::cout << currentOtherProp->intValues[k] << std::endl;
						}
					}
					break;
				case PLY_DATA_TYPE::PLY_UINT:
				case PLY_DATA_TYPE::PLY_UINT_32:
					if (!currentOtherProp->isList)
					{
						int dataNum = currentOtherEle->elemCount;
						for (int k = 0; k < dataNum; k++)
						{
							std::cout << currentOtherProp->uintValues[k] << std::endl;
						}
					}
					else
					{
						int dataNum = currentOtherEle->elemCount * currentOtherProp->listCount;
						for (int k = 0; k < dataNum; k++)
						{
							std::cout << currentOtherProp->uintValues[k] << std::endl;
						}
					}
					break;
				case PLY_DATA_TYPE::PLY_FLOAT:
				case PLY_DATA_TYPE::PLY_FLOAT_32:
				case PLY_DATA_TYPE::PLY_DOUBLE:
				case PLY_DATA_TYPE::PLY_FLOAT_64:
					if (!currentOtherProp->isList)
					{
						int dataNum = currentOtherEle->elemCount;
						for (int k = 0; k < dataNum; k++)
						{
							std::cout << currentOtherProp->doubleValues[k] << std::endl;
						}
					}
					else
					{
						int dataNum = currentOtherEle->elemCount * currentOtherProp->listCount;
						for (int k = 0; k < dataNum; k++)
						{
							std::cout << currentOtherProp->doubleValues[k] << std::endl;
						}
					}
					break;
				default:
					break;
				}
			}
		}
	};

private:
	PlyOtherData* mOtherData;

};


#endif /* !__PLYFILE_H__ */







