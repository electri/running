@echo off

set filename=running.db3

if not exist %filename% goto Error

echo .separator '	' > temp.sql
echo .import eventtype EventType >> temp.sql
echo .import intervaltype IntervalType >> temp.sql
echo .import weather Weather >> temp.sql

sqlite3 %filename% < temp.sql

if exist temp.sql erase temp.sql

goto Exit

:Error
echo File %filename% not found.

:Exit
pause
