/*!
 
 \file ASLog.m
 
 Implementation for the ASLog utility class.
 
 See the header file for the history, purposes and API documentation.
 
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

 */

#import "ASLog.h"

#pragma mark Static globals

/*! \var BOOL __sDebugLoggingOn
 \brief Controls logging by log...:/debugLog...: methods
 
 Flag boolean - if YES the log...: methods do log their messages. Is NO by default.
 
 Set to YES if the DEBUG_LOG_AUTO_ENABLE macro is defined or the "NSDebugEnabled" 
 environment variable exists and is set to YES. 
 
 This flag may also be changed by calling the  +setLogOn: method, allowing programmatic
 control of debug logging.
 
 It does not affect the warn...: methods.
 */
static BOOL __sDebugLoggingOn = NO;

/*! \var void (*__sCurLogFunc)(NSString *format, ...);
 \brief Function pointer to the logging function used by log...:/debugLog...:/warn...: methods.
 
 Function pointer - called by all the log...:/debugLog...:/warn...: methods to output their
 log text. Currently will be either NSLog() (the default) or QuietLog().
 
 QUietLog() can be selected at build time by defining the DEBUG_LOG_QUIET_ENABLE macro
 or programmatically with the +setQuietOn: method at runtime.
 */
static void (*__sCurLogFunc)(NSString *format, ...);

/*! Buffer to hold the path of the stderr stream on entry. Needed so we can restore
 stderr after redirection if required.
 */
static char __sStdErrPath[PATH_MAX+1];


/*!
 \brief Optional quieter substitute for NSLog() for logging output.
 
 NSLog() is very noisy - all that date, time and process clutter beginning each line. 
 QuietLog() prints only what you ask it to.
 
 QuietLog() has one significant difference to NSLog(). NSLog() output is intercepted by 
 the system and sent to /var/log/system.log. Logging is only directed to whatever stream 
 stderr is currently directed to.
 
 The ASLog logging/warning methods can be switched to use QuietLog() thse ways:
 
	Programmatically: call the +setQuietOn: class method
	
	At compile time by defining the DEBUG_LOG_QUIET_ENABLE macro
 
 QuietLog() created by Mark Dalrymple @ Big Nerd Ranch
 <https://www.bignerdranch.com/blog/a-quieter-log/>
 
 @param format - NSString * that holds the formatting string (vide NSLog()).
 
 @param ...	- variadic argument list.
 */
void QuietLog (NSString *format, ...)
{
    va_list argList;
    va_start (argList, format);
	
    NSString *message = [[NSString alloc] initWithFormat: format
											   arguments: argList];
	
	// no ARC in this build so...
	[message autorelease];
	
    va_end (argList);
	
    fprintf (stderr, "%s\n", [message UTF8String]);
	
}


#pragma mark Implementation starts here.

@implementation ASLog

#pragma mark Object management methods

/*!
 @brief Initialise our static variables
 
 ** NEVER CALL THIS PROGRAMMATICALLY **
 
 Method called once (and only once!) before a class object is used for the first time.
 
 Required because the class object is not programmatically instantiated with an 
 alloc/init pair and so would not otherwise get a chance to do any one time set up
 (as in this example, initialising static variables.
 
 This method tests whether to allow logging from the debug logging methods by testing two 
 conditions:
 
	 - Is the DEBUG_LOG_AUTO_ENABLE macro defined
	 - Is the environment variable NSDebugEnabled set to "YES"
 
 If either of these is true then it sets the static BOOL __sDebugLoggingOn to YES and
 so enables debug logging.
 
 In addition it checks whether DEBUG_LOG_QUIET_ENABLE is defined and if it is sets 
 __sCurLogFunc to point to QuietLog(), otherwise it points the variable at NSLog()
 
 The method also saves the output stream for stderr on entry to preserve it for later 
 restoration if the output stream is changed.
 
 */
+ (void) initialize
{
	// If DEBUG_LOG_AUTO_ENABLE is defined enable debug logging, irrespective of NSDebugEnabled
	#ifdef DEBUG_LOG_AUTO_ENABLE
		__sDebugLoggingOn = YES;
	#endif
	
	// If the environment var NSDebugEnabled is YES, enable debug logging
    char *env = getenv("NSDebugEnabled");
    env = (env == NULL ? "" : env);
    if(strcmp(env, "YES") == 0)
        __sDebugLoggingOn = YES;
	
	// initialise the logging function selection static boolean
	#ifdef DEBUG_LOG_QUIET_ENABLE
		__sCurLogFunc = QuietLog;
	#else
		__sCurLogFunc = NSLog;
	#endif
	
	// Save the current stderr output for later use
	int fd;
	fd = fileno(stderr);
	fcntl(fd, F_GETPATH, &__sStdErrPath);
	
}

#pragma mark Debug logging methods

/*!
 A simple substitute for NSLog(), called by the #ASDNSLog macro.
 
 The macro could simply call NSLog with the same parameters but then we would loose
 the ability to switch logging on or off.
 
 Logging is controlled via the the static BOOL __sDebugLoggingOn which is in turn
 controlled by the DEBUG_LOG_AUTO_ENABLE macro, the environment variable NSDebugEnabled
 or the control method +setlogOn: Logging is directed to whatever stream stderr is currently
 directed to.
 
 @param format - NSString * that holds the formatting string for NSLog().
 
 @param ...	- variadic argument list.
 */
+ (void)debugLog:(NSString *)format, ...;
{
    va_list ap;
    NSString *print;
    if(__sDebugLoggingOn == NO)
        return;
    va_start(ap, format);
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    __sCurLogFunc(@"%@", print);
    
    [print release];
}


/*!
 Enhances NSLog, Called by the #ASDLog macro.
 
 Calling this method via the macro enhances NSLog() by adding the source file name
 and line number of the call to the log output.
 
 Logging is controlled via the the static BOOL __sDebugLoggingOn which is in turn
 controlled by the DEBUG_LOG_AUTO_ENABLE macro, the environment variable NSDebugEnabled
 or the control method +setlogOn: Logging is directed to whatever stream stderr is currently
 directed to.
 
 @param sourceFile - c-string pointer holding the name of the source file.
 
 @param lineNumber - int holding the line number in the source file of the call.
 
 @param format - NSString * that holds the formatting string for NSLog().
 
 @param ...	- variadic argument list.
 */
+ (void)debugLog:(char *)sourceFile
	  lineNumber:(int)lineNumber
		  format:(NSString *)format, ...;
{
    va_list ap;
    NSString *print, *file;
    if(__sDebugLoggingOn == NO)
        return;
    va_start(ap, format);
    file = [NSString stringWithCString:sourceFile encoding:NSUTF8StringEncoding];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    __sCurLogFunc(@"%s:%d %@", [[file lastPathComponent] UTF8String], lineNumber, print);
    
    [print release];
}


/*!
 Enhances NSLog, Called by the #ASDFnLog macro.
 
 Calling this method via the macro enhances NSLog() by adding the source file name,
 line number and the name of the calling method/function to the log output.
 
 Logging is controlled via the the static BOOL __sDebugLoggingOn which is in turn
 controlled by the DEBUG_LOG_AUTO_ENABLE macro, the environment variable NSDebugEnabled
 or the control method +setlogOn: Logging is directed to whatever stream stderr is currently
 directed to.
 
 @param sourceFile - c-string pointer holding the name of the source file.
 
 @param lineNumber - int holding the line number in the source file of the call.
 
 @param functionName - c-string pointer holding the name of the calling method/function.
 
 @param format - NSString * that holds the formatting string for NSLog().
 
 @param ...	- variadic argument list.
 */
+ (void)debugLog:(char *)sourceFile
	  lineNumber:(int)lineNumber
		function:(char *)functionName
		  format:(NSString *)format, ...;
{
    va_list ap;
    NSString *print, *file, *function;
    if(__sDebugLoggingOn == NO)
        return;
    va_start(ap,format);
    file = [NSString stringWithCString:sourceFile encoding:NSUTF8StringEncoding];
    function = [NSString stringWithCString:functionName encoding:NSUTF8StringEncoding];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    __sCurLogFunc(@"%s:%d in %@ %@", [[file lastPathComponent] UTF8String], lineNumber, function, print);
    
    [print release];
}

#pragma mark Release logging methods

/*!
 Basic NSLog, Called by the #ASNSLog macro.
 
 Calling this method via the macro calls un-adorned  NSLog().
 
 Logging cannot be disabled. Logging is directed to whatever stream stderr is currently
 directed to.
 
 @param format - NSString * that holds the formatting string for NSLog().
 
 @param ...	- variadic argument list.
 */
+ (void)log:(NSString *)format, ...;
{
    va_list ap;
    NSString *print;
    va_start(ap, format);
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    __sCurLogFunc(@"%@", print);
    
    [print release];
}


/*!
 Enhances NSLog, Called by the #ASFlLog macro.
 
 Calling this method via the macro enhances NSLog() by adding the source file name
 and line number of the call to the log output.
 
 Logging cannot be disabled. Logging is directed to whatever stream stderr is currently
 directed to.
 
 @param sourceFile - c-string pointer holding the name of the source file.
 
 @param lineNumber - int holding the line number in the source file of the call.
 
 @param format - NSString * that holds the formatting string for NSLog().
 
 @param ...	- variadic argument list.
 */
+ (void)log:(char *)sourceFile
	  lineNumber:(int)lineNumber
		  format:(NSString *)format, ...;
{
    va_list ap;
    NSString *print, *file;
    va_start(ap, format);
    file = [NSString stringWithCString:sourceFile encoding:NSUTF8StringEncoding];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    __sCurLogFunc(@"%s:%d %@", [[file lastPathComponent] UTF8String], lineNumber, print);
    
    [print release];
}


/*!
 Enhances NSLog, Called by the #ASFnLog macro.
 
 Calling this method via the macro enhances NSLog() by adding the source file name,
 line number and the name of the calling method/function to the log output.
 
 Logging cannot be disabled. Logging is directed to whatever stream stderr is currently
 directed to.
 
 @param sourceFile - c-string pointer holding the name of the source file.
 
 @param lineNumber - int holding the line number in the source file of the call.
 
 @param functionName - c-string pointer holding the name of the calling method/function.
 
 @param format - NSString * that holds the formatting string for NSLog().
 
 @param ...	- variadic argument list.
 */
+ (void)log:(char *)sourceFile
	  lineNumber:(int)lineNumber
		function:(char *)functionName
		  format:(NSString *)format, ...;
{
    va_list ap;
    NSString *print, *file, *function;
    va_start(ap,format);
    file = [NSString stringWithCString:sourceFile encoding:NSUTF8StringEncoding];
    function = [NSString stringWithCString:functionName encoding:NSUTF8StringEncoding];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    __sCurLogFunc(@"%s:%d in %@ %@", [[file lastPathComponent] UTF8String], lineNumber, function, print);
    
    [print release];
}

#pragma mark Warning logging methods

/*!
 A simple substitute for NSLog(), called by the #ASNSWarn macro.
 
 Calling this method via the macro enhances NSLog() by adding the tag "WARNING:" 
 to the log output.
 
 The macro could simply call NSLog with the same paramters but then we would loose
 the "WARNING:" flag.
 
 Logging cannot be disabled. Logging is directed to whatever stream stderr is currently
 directed to.
 
 @param format - NSString * that holds the formatting string for NSLog().
 
 @param ...	- variadic argument list.
 */
+ (void)warn:(NSString *)format, ...;
{
    va_list ap;
    NSString *print;
    va_start(ap, format);
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    __sCurLogFunc(@"WARNING: %@", print);
    
    [print release];
}


/*!
 An enhanced substitute for NSLog(), called by the #ASWarn macro.
 
 Calling this method via the macro enhances NSLog() by adding the tag "WARNING:" and the
 source file name and line number of the call to the log output.
 
 Logging cannot be disabled. Logging is directed to whatever stream stderr is currently
 directed to.
 
 @param sourceFile - c-string pointer holding the name of the source file.
 
 @param lineNumber - int holding the line number in the source file of the call.
 
 @param format - NSString * that holds the formatting string for NSLog().
 
 @param ...	- variadic argument list.
 */
+ (void)warn:(char *)sourceFile
	  lineNumber:(int)lineNumber
		  format:(NSString *)format, ...;
{
    va_list ap;
    NSString *print, *file;
    va_start(ap, format);
    file = [NSString stringWithCString:sourceFile encoding:NSUTF8StringEncoding];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    __sCurLogFunc(@"WARNING: %s:%d %@", [[file lastPathComponent] UTF8String], lineNumber, print);
    
    [print release];
}


/*!
 An enhanced substitute for NSLog(), called by the #ASWarn macro.
 
 Calling this method via the macro enhances NSLog() by adding the tag "WARNING:" and the
 source file name, line number and calling method/function name to the log output.
 
 Logging cannot be disabled. Logging is directed to whatever stream stderr is currently
 directed to.
 
 @param sourceFile - c-string pointer holding the name of the source file.
 
 @param lineNumber - int holding the line number in the source file of the call.
 
 @param functionName - c-string pointer holding the name of the calling method/function.
 
 @param format - NSString * that holds the formatting string for NSLog().
 
 @param ...	- variadic argument list.
 */
+ (void)warn:(char *)sourceFile
	  lineNumber:(int)lineNumber
		function:(char *)functionName
		  format:(NSString *)format, ...;
{
    va_list ap;
    NSString *print, *file, *function;
    va_start(ap,format);
    file = [NSString stringWithCString:sourceFile encoding:NSUTF8StringEncoding];
    function = [NSString stringWithCString:functionName encoding:NSUTF8StringEncoding];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    __sCurLogFunc(@"WARNING: %s:%d in %@ %@", [[file lastPathComponent] UTF8String], lineNumber, function, print);
    
    [print release];
}

#pragma mark Control methods

/*!
 Programmatic control of logging for the debug logging methods.
 
 Even if logging is not enabled via the DEBUG_LOG_AUTO_ENABLE macro or the environment
 variable NSDebugEnabled you can enable/disable it at runtime with this call.
 
 This has no effect on the "Normal" or "Warning" logging methods and will have no
 effect if the debugging macros have been compiled out because the BUILD_WITH_DEBUG_LOGGING
 macro was not defined.
 
 @param logOn - BOOL, if YES then logging is enabled
 */
+ (void) setLogOn: (BOOL) logOn
{
    __sDebugLoggingOn=logOn;
}


/*!
 @brief Programmatic control of use of QuietLog() or NSLog().
 
 Switched the logging/warning methods between using NSLog() and QuietLog().
 The latter behaves exactly the same as NSLog() except:
 
	it is not intercepted and sent to /var/log/system.log
	
	it does not prepend the log line with all that date/time/process mess.
 
 @param quietOn - BOOL, if YES then logging functions will call QuietLog()
 */
+ (void) setQuietOn: (BOOL) quietOn
{
	if (quietOn) {
		__sCurLogFunc = QuietLog;
	} else {
		__sCurLogFunc = NSLog;
	}
}


/*!
 Redirect stderr output.
 
 By default stderr is sent to one of the /dev/tty streams. This method allows you 
 to redirect it to a convenient file.
 
 NOTE: The system intercepts NSLog() output and it ends up in /var/log/system.log.
 This call does not prevent the logging to system.log, simply gives you an additional,
 more accessible log, not full of irrelevant junk.
 
 @param filePath - NSString * holding either:
 
	- a full pathname in which case that path will be used.
	- a simple filename or partial pathname which means:
		if useAppDirAsBase is YES logging goes to /path/to/app/dir/\<filePath\>
		if useAppDirAsBase is NO logging goes to ~/\<filePath\>
	- nil which means logging goes to ~/Library/Logs/\<AppName\>.log and useAppDirAsBase is ignored
 */
+ (void)switchLoggingToFile:(NSString *)filePath fromAppDir:(BOOL)useAppDirAsBase
{
	NSString *logPath;
	
	// have we been passed a file or file path
	if (nil != filePath) {
		// yes, is it an abolute or relative path?
		if ('/' == [filePath characterAtIndex:0]) {
			// absolute: (begins with /)
			logPath = filePath;
		} else {
			// relative:
			if (!useAppDirAsBase) {
				//	assume is relative to home folder
				logPath = [NSHomeDirectory() stringByAppendingPathComponent:filePath];
			} else {
				//	relative to the app's diretcory
				NSBundle *appBundle = [NSBundle mainBundle];
				NSString *appBundlePath = [appBundle bundlePath];
				NSString *appDirPath = [appBundlePath stringByDeletingLastPathComponent];
				
				logPath = [appDirPath stringByAppendingPathComponent:filePath];
			}
			
		}
	} else {
		// No log file passed so create a log file in ~/Library/Logs
		
		// Get the absolute path to the Logs folder
		NSString *logDirPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Library/Logs/"];
		
		// Get our application's name
		NSProcessInfo *pi = [NSProcessInfo processInfo];
		NSString *appName = [pi processName];
		/*
			 // Alternative methof of getting application name:
			 NSBundle *appBundle = [NSBundle mainBundle];
			 NSString *appName = [appBundle objectForInfoDictionaryKey:@"CFBundleName"];
		 */
		
		// Create log name
		NSString *logName = [appName stringByAppendingString:@".log"];
		logPath = [logDirPath stringByAppendingPathComponent:logName];
	}
	
	// we have a full path for out log, reopen stderr to use that file
	freopen([logPath fileSystemRepresentation], "a", stderr);
}

/*!
 Restore the original destination of redirected stderr.
 
 Uses the stored value for the original destination of stderr (saved in __sStdErrPath)
 to reset stderr output to that stream.
 */
+ (void)restoreStdErr
{
	freopen(__sStdErrPath, "a", stderr);
}


@end
