/*================================================================================================*/
/*
 *
 *	Copyright 2018, 2023-2024 Avid Technology, Inc.
 *	All rights reserved.
 *	
 *	This file is part of the Avid AAX SDK.
 *	
 *	The AAX SDK is subject to commercial or open-source licensing.
 *	
 *	By using the AAX SDK, you agree to the terms of both the Avid AAX SDK License
 *	Agreement and Avid Privacy Policy.
 *	
 *	AAX SDK License: https://developer.avid.com/aax
 *	Privacy Policy: https://www.avid.com/legal/privacy-policy-statement
 *	
 *	Or: You may also use this code under the terms of the GPL v3 (see
 *	www.gnu.org/licenses).
 *	
 *	THE AAX SDK IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
 *	EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
 *	DISCLAIMED.
 *
 */

/**  
 *	\file   AAX_PreStructAlignmentHelper.h
 *
 *	\brief Helper file for data alignment macros
 */ 
/*================================================================================================*/

#if defined (AAX_SDK__PRE_STRUCT_ALIGNMENT_HELPER_DID_PUSH_A_CHANGE)
	#warning nested struct alignment directives are not tested
#endif

#if defined (__clang__)
	#if __has_warning ("-Wpragma-pack")
		#define AAX_SDK__PRE_STRUCT_ALIGNMENT_HELPER_DID_PUSH_A_CHANGE 1
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wpragma-pack"
	#endif
#endif
