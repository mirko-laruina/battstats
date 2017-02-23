# Battstats
Battstats is a simple utility to track your battery

	./battstats start : starts the "daemon"
	./battstats start clean : starts and cleans the log file
	./battstats stop : obvious
	./battstats : prints information about battery percentage and time remaining

battstats.log contains information about computer state (charging or discharging), time on battery and relative battery percentage.
Measurement interval has to be set manually in BattStats.h.
