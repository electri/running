set filename=running.db
if not exist %filename% goto Error
sqlite3 %filename% < upgrade_0-1.sql
sqlite3 %filename% < upgrade_0-1_init_it_IT.sql
goto Exit

:Error
echo File %filename% not found.

:Exit
pause
