# Battstats
Battstats is a simple utility to track your battery usage.

	./battstats start : starts the 'daemon'
	./battstats start clean : starts and cleans the log file
	./battstats start 2 : starts with 2 minutes (or whetever it is set) measurement interval
	./battstats start clean 2 : same as before but it cleans log
	./battstats stop : obvious
	./battstats : prints information about battery percentage and time remaining
	./battstats help : prints this help screen

battstats.log contains information about computer state (charging or discharging), time on battery and relative battery percentage.
