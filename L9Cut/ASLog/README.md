# ASLog #

Logging utilities for Cocoa

### About ASLog ###

ASLog is a library that augments Cocoa's NSLog().

* It creates debug logging commands as macros which only compile to functional code
  in debugging builds. In release builds these macros are compiled out.
* The basic NSLog status/origin information is enhanced by the addition of source 
  file name and line, (and optionally the method/function) from which the call comes.
* It is possible to switch the logging from the default destination of stderr to a 
  user specified file.
* Optionally use QuietLog() function which doesn't emit the date/time/process
  noise that NSLog() does.
* Automatically add warning message to output.

### History ###

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

### How does it work? ###

#### Basic Concept ####

All ASLog functions (with the exception of `QuietLog()`) are class methods of
the ASLog class. With the exception of the methods to enable/disable logging 
or quiet logging the methods are not intended to be called directly but instead 
via convenience macros that do a lot of the work for you.

There are three groups of macros. For all these, the parameters are exactly as 
expected by `NSLog()`.

1. Debug logging macros. These all begin with `ASD` and will be compiled out 
   if the `BUILD_WITH_DEBUG_LOGGING` macro is not defined. If compiled in:
	* They can be enabled at build time if the `DEBUG_LOG_AUTO_ENABLE` macro is
	  defined.
	  
	* They can be enabled at launch if the `NSDebugEnabled` environment 
	  variable exists and is set to `'YES'`.
	  
	* They can be enabled/disabled on the fly by the `ASDLogOn` and `ASDLogOff`
	  macros.
	* Switched between QuietLog()/NSLog() by the `ASDQuietLogOn` and
	  `ASDQuietLogOff` macros.
	  
		(You could also use the class methods `+setLogOn:` and `+setQuietOn:` 
		to do the same thing but these would not be compiled out in non-debugging
		builds.
	
	They are:
	
	-  `ASDNSLog(s, ...)`
		No enhancements to NSLog, but will be compiled out in release builds.
		
	-	`ASDLog(s, ...)`
		NSLog + logs the sourcefile and line number
		
	-	`ASDFnLog(s, ...)`
		NSLog + logs the sourcefile, line number and calling method

2. Standard logging macros. These all begin with `AS`. They will not be compiled 
   out in release builds. These cannot be disabled, but their output can be 
   redirected and switched between NSLog() and QuietLog() output.
   
	They are:
	
	-  `ASNSLog(s, ...)`
		NSLog, unadorned
		
	-	`ASFlLog(s, ...)`
		NSLog + logs the sourcefile and line number
		
	-	`ASFnLog(s, ...)`
		NSLog + logs the sourcefile, line number and calling method

3. Warning logging macros. These all begin with `AS` and end with `Warn`. They 
   will not be compiled out in release builds. These cannot be disabled but 
   their output can be redirected and switched between NSLog() and QuietLog() 
   output.
   
   All begin their output with the string 'WARNING' so they are easily 
   visible even in a busy log file.
   
	They are:
	
	-  `ASNSWarn(s, ...)`
		NSLog + "WARNING"
		
	-	`ASWarn(s, ...)`
		NSLog + "WARNING" + logs the sourcefile and line number
		
	-	`ASFnWarn(s, ...)`
		NSLog + "WARNING" + logs the sourcefile and line number and calling method

#### Enabling and Disabling ASLog Functions ####

1. If the `BUILD_WITH_DEBUG_LOGGING` macro is not defined, the debug logging 
macros are compiled out.

2. If the `DEBUG_LOG_AUTO_ENABLE` macro is defined or the `NSDebugEnabled` 
   environment variable exists and is set to `'YES'` the debug logging macros 
   will print their log lines.

3. If the debug logging macros are compiled in output can be enabled/disabled
   at runtime by calling the `ASDLogOn` or `ASDLogOff` macros (which can be 
   compiled out) or by the class method `+setLogOn:` which cannot be.
   
4. The ASLog logging methods can be optionally switched to use QuietLog() by 
   use of the `ASDQuietOn` or `ASDQuietOff` macros (which can be compiled out) 
   or by the class method `+setQuietOn:` which cannot be.
   
#### QuietLog() ####

Optional quieter substitute for NSLog() for logging output.

NSLog() is very noisy - all that date, time and process clutter beginning each
line. QuietLog() prints only what you ask it to.

QuietLog() has one significant difference to NSLog(). NSLog() output is
intercepted by the system and sent to /var/log/system.log. Logging is only
directed to whatever stream stderr is currently directed to.

The ASLog logging/warning methods can be switched to use QuietLog() thse ways:

	- Programmatically: call the `+setQuietOn:` class method

	- At compile time by defining the `DEBUG_LOG_QUIET_ENABLE` macro

QuietLog() created by Mark Dalrymple @ Big Nerd Ranch
[A Quieter Log](https://www.bignerdranch.com/blog/a-quieter-log/)

Call as: `QuietLog (NSString *format, ...);`

with the same parameters as NSLog().

### Contribution guidelines ###

The source is deliberately non-ARC as I use this in projects that have to 
support back to 10.4

Please let me know, via messaging or pull requests if you have any suggestions
to improve ASLog.

### License ###

GNU Lesser General Public License

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

