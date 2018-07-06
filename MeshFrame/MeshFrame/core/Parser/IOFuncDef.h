#pragma once
#include <stdio.h>

#define MAX_LINE_SIZE 1024
#define MAX_TRAIT_STRING_SIZE 512
#define MAX_TOKEN_STRING_SIZE 128
#define MAX_DATA_STRING_SIZE 64

#ifdef _WIN32
#define  SAFE_SPRINT sprintf_s
#define  SAFE_STRCAT strcat_s
#define  SAFE_FOPEN fopen_s
#define	 SAFE_STRCPY strcpy_s
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