#!/bin/bash

filename=running.db3

if [[ ! -n $filename ]]; then rm $filename; fi

sqlite3 $filename < create_table_shoemaker.sql
sqlite3 $filename < create_table_shoemodel.sql
sqlite3 $filename < create_table_shoe.sql
sqlite3 $filename < create_table_weather.sql
sqlite3 $filename < create_table_eventtype.sql
sqlite3 $filename < create_table_event.sql
sqlite3 $filename < create_table_intervaltype.sql
sqlite3 $filename < create_table_interval.sql
