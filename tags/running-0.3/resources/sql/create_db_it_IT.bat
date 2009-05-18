set filename=running.db
if exist %filename% erase %filename%
sqlite3 %filename% < create.sql
sqlite3 %filename% < create_init_it_IT.sql
pause
