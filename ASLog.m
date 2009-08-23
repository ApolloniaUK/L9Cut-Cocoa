/*!
 
 \file ASLog.m
 
 Implementation for the ASLog utility class.
 
*/

#import "ASLog.h"

#pragma mark Static globals

/*! Flag boolean - if YES the log...: methods do log their messages. Is NO by default
 unless the "NSDebugEnabled" environment variable exists and is set to YES. This flag
 can be changed by calls to setLogOn: allowing programmatic control of debug logging.
 
 It does not affect the warn...: methods.
 */
static BOOL __sDebugLoggingOn = NO;

/*! Buffer to hold the path of the stderr stream on entry. Needed so we can restore
 stderr after redirection if required.
 */
static char __sStdErrPath[PATH_MAX+1];


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
	
	// Save the current stderr output for later use
	int fd;
	fd = fileno(stderr);
	fcntl(fd, F_GETPATH, &__sStdErrPath);
	
}

#pragma mark Debug logging methods

/*!
 A simple substitute for NSLog(), called by the #ASDNSLog macro.
 
 The macro could simply call NSLog with the same paramters but then we would loose
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
    
    NSLog(@"%@", print);
    
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
    file = [NSString stringWithCString:sourceFile];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    NSLog(@"%s:%d %@", [[file lastPathComponent] UTF8String], lineNumber, print);
    
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
    file = [NSString stringWithCString:sourceFile];
    function = [NSString stringWithCString:functionName];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    NSLog(@"%s:%d in %@ %@", [[file lastPathComponent] UTF8String], lineNumber, function, print);
    
    [print release];
}

#pragma mark Release logging methods

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
    file = [NSString stringWithCString:sourceFile];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    NSLog(@"%s:%d %@", [[file lastPathComponent] UTF8String], lineNumber, print);
    
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
    file = [NSString stringWithCString:sourceFile];
    function = [NSString stringWithCString:functionName];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    NSLog(@"%s:%d in %@ %@", [[file lastPathComponent] UTF8String], lineNumber, function, print);
    
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
    
    NSLog(@"WARNING: %@", print);
    
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
    file = [NSString stringWithCString:sourceFile];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    NSLog(@"WARNING: %s:%d %@", [[file lastPathComponent] UTF8String], lineNumber, print);
    
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
    file = [NSString stringWithCString:sourceFile];
    function = [NSString stringWithCString:functionName];
    print = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    
    NSLog(@"WARNING: %s:%d in %@ %@", [[file lastPathComponent] UTF8String], lineNumber, function, print);
    
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
 Redirect stderr output.
 
 By default stderr is sent to one of the /dev/tty streams. This method allows you 
 to redirect it to a convenient file.
 
 NOTE: The system intercepts NSLog() output it and it ends up in /var/log/system.log.
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
