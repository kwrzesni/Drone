# arduino-BasicTimer
Basic Timer Library For Aduino Cores


### A Note on millis() and rollover
This library depends on the Arduino millis() function for timing
information, and if the millis() function is not accurate, neither 
will these timers.  The millis() timer stores its data as a 32-bit
unsigned value and will rollover after ~50 days.  In most use cases,
this is of now issue.  In order to keep most functionality lightweight 
and simple, these timer libraries make no special accounting for a roll
over case.  If a roll-over occurs, due to the nature of unsigned arithmetic,
the elapsed time will all of a sudden become a very large value and the 
timer will automatically become expired.  This is considered a safe operation
in the use cases I wrote the library for (making an LED blink in time or 
scanning switches for user input every 50 milliseconds, etc..) and
one short period every 50 days is not of concern.  If it is for you,
another more robust timing method is recommended.  

