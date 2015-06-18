//
//  DCFPlatform.h
//  DCFoundation
//
//  Created by Rob Dotson  on 4/10/15.
//  Copyright (c) 2015 Design Complex. All rights reserved.
//

#ifndef __DCFPlatform__
#define __DCFPlatform__

#include <DCFoundation/DCFMacros.h>
#include <DCFoundation/DCFDefines.h>

#pragma mark - APPLE

#if !defined( __DARWIN__ )
#if defined( __APPLE__ ) && defined( __MACH__ )

#define __DARWIN__

#endif // defined( __APPLE__ )
#endif // defined( __DARWIN__ )

#pragma mark - UNIX

#pragma mark - Windows

#if !defined( __WINDOWS__ )
#if defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32 ) || defined( __WIN32__ ) \
|| defined( WIN64 ) || defined( _WIN64 ) || defined( __WIN64 ) || defined( __WIN64__ ) \
|| defined( __MINGW32__ ) || defined( __MINGW64__ )

#define __WINDOWS__

#endif // defined( WIN32 )...
#endif // defined( __WINDOWS__ )

#include <DCFoundation/DCFSingleton.h>

/*! \file */

DCF_NAMESPACE_DEFINE( DCF ) DCF_NAMESPACE_DEFINE( System )

#pragma mark - Class Declaration

class DCF_VISIBLE Platform  {
        DCFSingletonDeclare( Platform );
    
public:
    virtual size_t numberOfCores() const = 0;
};

DCF_NAMESPACE_END DCF_NAMESPACE_END

#pragma mark Concrete Type Declarations

typedef DCF::System::Platform DCFPlatform;

#if defined( __DARWIN__ )

#include <DCFoundation/DCFDarwinPlatform.h>

#elif defined( __WINDOWS__ )

#include <DCFoundation/DCFWindowsPlatform.h>

#elif defined( __LINUX__ )
#include <DCFoundation/DCFLinuxPlatform.h>

#else

#error "Unknown platform!"

#endif // Platforms

#endif // __DCFPlatform__
