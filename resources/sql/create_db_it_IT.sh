#!/bin/bash
filename=running.db
if [[ ! -n $filename ]]; then rm $filename; fi
sqlite3 $filename < create.sql
sqlite3 $filename < create_init_it_IT.sql
