/*!
 ASLog.h
 
 \file ASLog.h
 
 \brief Interface for ASLog utility class
 
 \author Copyright (c) 2008 - 2015 by Alan Staniforth
 
 \date 2008-10-12
 
 \version 1.1.0
 
 This abstract class enhances the functionality of NSLog in several ways:
 
 -#	It creates debug logging commands as macros which only compile to functional code
	in debugging builds. In release builds these macros are compiled out.
 -# The basic NSLog status/origin information is enhanced by the addition of source 
	file name and line, (and optionally the function) from which the call comes.
 -# It is possible to switch the logging from the default destination of stderr to a 
	user specified file.
 
 This code is based on a number of sources and ideas.The general idea of using
 an enhanced, NSLog, implemented as macros which could be compiled out came from:
 
 [A Better NSLog()](http://borkware.com/rants/agentm/mlog/)
 
 Adam Knight then presented the same idea, enhanced a little further and already 
 packaged up in a neat utility class:
 
 [Better Logging](http://www.hopelessgeek.com/2005/11/18/better-logging)
 (now [Better Logging - Internet Archive](https://web.archive.org/web/20110712220001/http://www.hopelessgeek.com/2005/11/18/better-logging))
 
 I started with Adam's class and enhanced the macros as suggested on:
 
 [Stupid C++ Tricks: Adventures in Assert](http://powerof2games.com/node/10)
 (now [Stupid C++ Tricks: Adventures in Assert - Internet Archive](https://web.archive.org/web/20081101231431/http://www.powerof2games.com/node/10))
 
 to be absolutely sure the macros were compiled out with no side effects. I added
 a "Warning" variant, which would not compile out as suggested in:
 
 [Uncle Jens’s Coding Tips](http://mooseyard.com/Jens/2007/05/uncle-jenss-coding-tips/)
 (now [Uncle Jens’s Coding Tips - Internet Archive](https://web.archive.org/web/20090319202059/http://mooseyard.com/Jens/2007/05/uncle-jenss-coding-tips/))
 
 and the idea of reassigning stderr came from:
 
 [NSLogToFile](http://www.cocoadev.com/index.pl?NSLogToFile)
 (now [NSLogToFile - Internet Archive](https://web.archive.org/web/20120303210003/http://www.cocoadev.com/index.pl?NSLogToFile))
 
 I personally added the versions of the macros that do "ordinary" enhanced logging
 and do not get compiled out.
 
 License
 =======
 	
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
	USA

 Changes
 =======
 
 2009-08-23 -	Changed +switchLoggingToFile: to +switchLoggingToFile:fromAppDir:
				to allow the use of the application directory as a base for the
				logging file path.
 2009-08-23 -	Added #define ASLogVersion to track version
 2015-03-30 -	Added QuietLog() - a logging substitute for NSLog() that does 
 				not generate so much noise.
 2015-04-05 -	Added ability to switch logging functions between NSLog() and 
 				QuietLog()
 2015-04-08 - 	Added simple call-through to main logging function without file 
 				or line number info for the standard logging methods. Also 
 				defined accompanying macro.
 
 */

#import <Foundation/NSDebug.h>
#import <Cocoa/Cocoa.h>



#pragma mark Macro defintions

/*! \def ASLogVersion
 @brief String holding version number for the utilities
 */
#define ASLogVersion "1.0.1"

/*!
 \name Debug Logging macros. 
 @relates ASLog
 
 Convenience interface to ASLog Debug Logging methods
 
 - Only compiled in when BUILD_WITH_DEBUG_LOGGING is defined.
 - Only fire when either DEBUG_LOG_AUTO_ENABLE is defined or the environment
	variable NSDebugEnabled exists and is set to YES
 
 */
//@{

	/*! \def ASDLogOn
	 @brief Enables debug logging at runtime, compiled out in release builds
	 
	 \def ASDLogOff
	 @brief Disables debug logging at runtime, compiled out in release builds
	 
	 \def ASDQuietLogOn
	 @brief Enables quiet logging at runtime, compiled out in release builds
	 
	 \def ASDQuietLogOff
	 @brief Disables quiet logging at runtime, compiled out in release builds
	 
	 \def ASDNSLog
	 @brief No enhancements to NSLog, but will be compiled out in release builds
	 
	 \def ASDLog
	 @brief NSLog + logs the sourcefile and line number
	 
	 \def ASDFnLog
	 @brief NSLog + logs the sourcefile and line number and calling method
	 */
#ifdef BUILD_WITH_DEBUG_LOGGING
	// BUILD_WITH_DEBUG_LOGGING is defined, compile the macros in
	#define ASDLogOn() do { [ASLog setLogOn:YES]; } while (0)
	#define ASDLogOff() do { [ASLog setLogOn:NO]; } while (0)
	#define ASDQuietLogOn() do { [ASLog setQuietOn:YES]; } while (0)
	#define ASDQuietLogOff() do { [ASLog setQuietOn:NO]; } while (0)
	#define ASDNSLog(s, ...) do { [ASLog debugLog:(s),##__VA_ARGS__]; } while (0)
	#define ASDLog(s, ...) do { [ASLog debugLog:__FILE__ lineNumber:__LINE__ format:(s),##__VA_ARGS__]; } while (0)
	#define ASDFnLog(s, ...) do { [ASLog debugLog:__FILE__ lineNumber:__LINE__ function:(char*)__FUNCTION__ format:(s),##__VA_ARGS__]; } while (0)
#else
	// NOOP definitions of the debug logging macros
	#define ASDLogOn() do { (void)sizeof(YES); } while (0)
	#define ASDLogOff() do { (void)sizeof(YES); } while (0)
	#define ASDQuietLogOn() do { (void)sizeof(YES); } while (0)
	#define ASDQuietLogOff() do { (void)sizeof(YES); } while (0)
	#define ASDNSLog(s, ...) do { (void)sizeof(s); } while (0)
	#define ASDLog(s, ...) do { (void)sizeof(s); } while (0)
	#define ASDFnLog(s, ...) do { (void)sizeof(s); } while (0)
#endif

//@} (Debug Logging macros)

/*!
 \name Normal Logging macros.
 @relates ASLog
 
 Convenience interface to ASLog Normal Logging methods
 
 - Still have NSLog enhancements.
 - Not compiled out in release builds.
 
 */
//@{

/*! \def ASNSLog
 @brief NSLog, unadorned
 */
#define ASNSLog(s, ...) do { [ASLog log:(s),##__VA_ARGS__]; } while (0)

/*! \def ASFlLog
 @brief NSLog + logs the sourcefile and line number
 */
#define ASFlLog(s, ...) do { [ASLog log:__FILE__ lineNumber:__LINE__ format:(s),##__VA_ARGS__]; } while (0)

/*! \def ASFnLog
 @brief NSLog + logs the sourcefile and line number and calling method
 */
#define ASFnLog(s, ...) do { [ASLog log:__FILE__ lineNumber:__LINE__ function:(char*)__FUNCTION__ format:(s),##__VA_ARGS__]; } while (0)

//@} (Normal Logging macros)


/*!
 \name Warning Logging macros.
 @relates ASLog
 
 Convenience interface to ASLog Warning methods
 
 - Still have NSLog enhancements.
 - Not compiled out in release builds.
 - Obvious in a busy log as every line contains "WARNING"
 */
//@{

/*! \def ASNSWarn
 @brief NSLog + "WARNING"
 */
#define ASNSWarn(s, ...) do { [ASLog warn:(s),##__VA_ARGS__]; } while (0)

/*! \def ASWarn
 @brief NSLog + "WARNING" + logs the sourcefile and line number
 */
#define ASWarn(s, ...) do { [ASLog warn:__FILE__ lineNumber:__LINE__ format:(s),##__VA_ARGS__]; } while (0)

/*! \def ASFnWarn
 @brief NSLog + "WARNING" + logs the sourcefile and line number and calling method
 */
#define ASFnWarn(s, ...) do { [ASLog warn:__FILE__ lineNumber:__LINE__ function:(char*)__FUNCTION__ format:(s),##__VA_ARGS__]; } while (0)

//@} (Warning Logging macros)

#pragma mark Prototypes

/*! \fn QuietLog (NSString *format, ...)
 @brief A quieter NSLog()
 */
extern void QuietLog (NSString *format, ...);


#pragma mark Class interface

/*!
 \brief A better NSLog...
 
 
 This abstract class enhances the functionality of NSLog in several ways:
 
 -#	It creates debug logging commands as macros which only compile to functional code
 in debugging builds. In release builds these macros are compiled out.
 -# The basic NSLog status/origin information is enhanced by the addition of source 
 file name and line, (and optionally the function) from which the call comes.
 -# It is possible to switch the logging from the default destination of stderr to a 
 user specified file.
 
 To enable logging, set the environment variable NSDebugEnabled to YES in the 
 "Variables to be set in the environment" section of the "Arguments" tab of the 
 "Edit Active Target..." window.
 This is imported in Foundation/NSDebug.h as a flag for other debugging tools. 
 (The necessary header is imported in this class's header)
 
 */

@interface ASLog : NSObject {
}

/*!
 \name Debug Logging methods. 
 - Only fire when either DEBUG_LOG_AUTO_ENABLE is defined or the environment
 variable NSDebugEnabled exists and is set to YES
 */
//@{

//! @brief Plain NSLog, but will be compiled out in release builds
+ (void)debugLog:(NSString *)format, ...;

//! @brief NSLog, also logs source file and line number
+ (void)debugLog:(char *)sourceFile lineNumber:(int)lineNumber format:(NSString *)format, ...;

//! @brief NSLog, also logs source file, line number and calling method
+ (void)debugLog:(char *)sourceFile lineNumber:(int)lineNumber function:(char *)functionName format:(NSString *)format, ...;

//@} (Debug Logging methods)

/*!
 \name Enhanced Normal Logging methods. 
 - Always fire
 */
//@{

//! @brief NSLog, nothing else.
+ (void)log:(NSString *)format, ...;

//! @brief NSLog, also logs source file and line number
+ (void)log:(char *)sourceFile lineNumber:(int)lineNumber format:(NSString *)format, ...;

//! @brief NSLog, also logs source file, line number and calling method
+ (void)log:(char *)sourceFile lineNumber:(int)lineNumber function:(char *)functionName format:(NSString *)format, ...;

//@} (Enhanced Normal Logging methods)

/*!
 \name WARNING Logging methods. 
 - Always fire
 - Always have 'WARNING' in the output so easier to spot in busy log
 */
//@{

//! @brief Plain NSLog, but adds "WARNING"
+ (void)warn:(NSString *)format, ...;

//! @brief NSLog, adds "WARNING" and also logs source file and line number
+ (void)warn:(char *)sourceFile lineNumber:(int)lineNumber format:(NSString *)format, ...;

//! @brief NSLog, adds "WARNING" and also logs source file, line number and calling method
+ (void)warn:(char *)sourceFile lineNumber:(int)lineNumber function:(char *)functionName format:(NSString *)format, ...;

//@} (WARNING Logging methods)

/*!
 \name Control methods. 
 - Used to enable/disable logging for debugging methods and to redirect log output
 */
//@{

//! @brief Enables/Disables logging at runtime for the debug logging methods
+ (void)setLogOn: (BOOL) logOn;

//! @brief Switches logging methods between using NSLog() or QuietLog()
+ (void) setQuietOn: (BOOL) quietOn;

//! @brief Switches stderr to logging to a user specified file
+ (void)switchLoggingToFile:(NSString *)filePath fromAppDir:(BOOL)useAppDirAsBase;

//! @brief Switches stderr back to logging to default output stream
+ (void)restoreStdErr;

//@} (Control methods)

@end
