/*!
*	\file Lib_DP_Types.h
*	\brief This file contains Data types supported by drivers in both 32 bit and 64 bit.  
*
*   \author Ruchitha.konreddy
*   \date  11 march 2023
*
*   \copyright Copyright (C) 2022 Data Patterns (India). Ltd. \n
*	All Rights Reserved.\n
*	Address: Plot # H9, Fourth Main Road, SIPCOT IT Park, Siruseri, \n
*           Off Rajiv Gandhi Salai (OMR), Pudupakkam P.O., \n
*           Chennai-603103 | India\n
*	Website : http://www.datapatternsindia.com/\n
*	Phone: 91-44-4741-4000\n
*	FAX: 91-44-4741-4444 \n
*/
/**
* \file dp_types.h
* \brief contains the data types supported by drivers in both 32 bit and 64 bit 
*
* This file contains the data types supported by drivers in both 32 bit and 64 bit 
*
* \version 1.01
*
* \Copyright © 2022 Data Patterns (India) Ltd. 
*/

#ifndef _DP_TYPES_H_
#define _DP_TYPES_H_

typedef unsigned char         	U8BIT;				/*!< unsigned 8 bit datatype */
typedef unsigned short        	U16BIT;				/*!< unsigned 16 bit datatype */
typedef unsigned int          	U32BIT;				/*!< unsigned 32 bit datatype */

typedef char           		  	S8BIT;				/*!< signed 8 bit datatype */
typedef short                 	S16BIT;				/*!< signed 16 bit datatype */
typedef int                   	S32BIT;				/*!< signed 32 bit datatype */

typedef unsigned char *        	PU8BIT;				/*!< unsigned 8 bit pointer datatype*/
typedef unsigned short *       	PU16BIT;				/*!< unsigned 16 bit pointer datatype */
typedef unsigned int *         	PU32BIT;				/*!< unsigned 32 bit pointer datatype */

typedef char *         		  	PS8BIT;				/*!< signed 8 bit pointer datatype */
typedef short *                	PS16BIT;				/*!< signed 16 bit pointer datatype */
typedef int *                   PS32BIT;				/*!< signed 32 bit pointer datatype */

typedef float					FSINGLE;				/*!< float datatype */
typedef double					FDOUBLE;				/*!< double datatype */

typedef float *					PFSINGLE;				/*!< float pointer datatype */
typedef double *				PFDOUBLE;				/*!< double pointer datatype */

typedef void * 					DP_DRV_HANDLE;				/*!< void pointer datatype */
typedef void * 					PVPTR;				/*!< void pointer datatype */

typedef unsigned int 			DPPOINTER32;				/*!< unsigned 32 bit datatype */
/*Arch = x86 and MS VC++ version <= 6.0 then long long is not supported*/
#if (defined(_M_IX86) || defined(_X86_)) && (_MSC_VER <= 1200)

//typedef __int64					S64BIT;				/*!< signed 64 bit datatype */
//typedef unsigned __int64		U64BIT;				/*!< unsigned 64 bit datatype */

//typedef __int64 *				PS64BIT;				/*!< signed 64 bit pointer datatype */
//typedef unsigned __int64*		PU64BIT;				/*!< unsigned 64 bit pointer datatype */

#define STDCALL __stdcall

#else

typedef long long 	 			S64BIT;				/*!< signed 64 bit datatype */
typedef unsigned long long 		U64BIT;				/*!< unsigned 64 bit datatype */

typedef long long * 			PS64BIT;				/*!< signed 64 bit pointer datatype */
typedef unsigned long long *	PU64BIT;				/*!< unsigned 64 bit pointer datatype */

#if !(defined(_MSC_VER)) && !(defined(__MINGW32__) || defined(__MINGW64__)) /*If complier is not msvc and MinGW*/
#define STDCALL
#else /*If MSVC version is > 6.0*/ 
#define STDCALL __stdcall
#endif
#endif /*#if (defined(_M_IX86) || defined(_X86_)) && (_MSC_VER <= 1200)*/

#if defined(_MSC_VER)
#define PRAGMA(X) 		__pragma(X)
#define PRAGMA_PACK_PUSH(n)	PRAGMA(pack(push,n))
#define PRAGMA_PACK_POP()	PRAGMA(pack(pop))
#define PACK_ATTRIBUTE
#elif  defined(__GNUC__)
#define PRAGMA_PACK_PUSH(n)
#define PRAGMA_PACK_POP()
#define PACK_ATTRIBUTE 		__attribute__((packed,aligned(1)))
#else

  #error "Unsupported compiler"

#endif
#endif /*#ifndef _DP_TYPES_H_*/
