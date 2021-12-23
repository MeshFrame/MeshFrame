#pragma once
#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif 
#endif
#include <stdio.h>
#include <string.h>

#define MAX_LINE_SIZE 1024
#define MAX_TRAIT_STRING_SIZE MAX_LINE_SIZE
#define MAX_TOKEN_STRING_SIZE MAX_LINE_SIZE
#define MAX_DATA_STRING_SIZE MAX_LINE_SIZE

#ifdef _WIN32
#define  SAFE_SPRINT sprintf_s
#define  SAFE_STRCAT(dst, src) strcat_s(dst, MAX_LINE_SIZE, src)
#define  SAFE_FOPEN fopen_s
#define	 SAFE_STRCPY(dst, src) strcpy_s(dst, MAX_LINE_SIZE, src)
#define	 SAFE_SSCANF sscanf_s

#elif __ANDROID__
#define  SAFE_SPRINT snprintf
#define  SAFE_STRCAT strncat
#define  SAFE_FOPEN fopen
#define	 SAFE_STRCPY strncpy

#elif __linux__
#define  SAFE_SPRINT snprintf
#define  SAFE_STRCAT strncat
#define  SAFE_FOPEN fopen
#define	 SAFE_STRCPY strncpy
#elif __unix__ 
#define  SAFE_SPRINT snprintf
#define  SAFE_STRCAT strncat
#define  SAFE_FOPEN fopen
#define	 SAFE_STRCPY strncpy
#endif 