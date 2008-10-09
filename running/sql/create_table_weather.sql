CREATE TABLE Weather
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	Description TEXT,
	-- constraints
	UNIQUE (Description) ON CONFLICT ROLLBACK,
	CHECK(LENGTH(Description)>0) ON CONFLICT ROLLBACK
);
