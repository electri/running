CREATE TABLE EventType
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	Description TEXT,
	-- additional info
	HasMedal TEXT,
	HasIntervals TEXT,
	-- constraints
	UNIQUE (Description) ON CONFLICT ROLLBACK,
	CHECK(LENGTH(Description)>0) ON CONFLICT ROLLBACK
);
