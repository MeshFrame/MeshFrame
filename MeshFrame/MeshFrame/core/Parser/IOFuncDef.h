#pragma once
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include <stdio.h>
#include <string.h>

#define MAX_LINE_SIZE 1024
#define MAX_TRAIT_STRING_SIZE 512
#define MAX_TOKEN_STRING_SIZE 256
#define MAX_DATA_STRING_SIZE 128

#ifdef _WIN32
#define  SAFE_SPRINT sprintf_s
#define  SAFE_STRCAT(dst, src, size) strcat_s(dst, size, src)
#define  SAFE_FOPEN fopen_s
#define	 SAFE_STRCPY(dst, src, size) strcpy_s(dst, size, src)
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