PRAGMA user_version = 1;



CREATE TABLE CfgDistanceUnit
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	Description TEXT,
	-- constraints
	UNIQUE (Description) ON CONFLICT ROLLBACK,
	CHECK(LENGTH(Description)>0) ON CONFLICT ROLLBACK
);

INSERT INTO CfgDistanceUnit VALUES(1, 'Mi');

INSERT INTO CfgDistanceUnit VALUES(2, 'Km');

CREATE TABLE CfgWeightUnit
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	Description TEXT,
	-- constraints
	UNIQUE (Description) ON CONFLICT ROLLBACK,
	CHECK(LENGTH(Description)>0) ON CONFLICT ROLLBACK
);

INSERT INTO CfgWeightUnit VALUES(1, 'Li');

INSERT INTO CfgWeightUnit VALUES(2, 'Kg');

CREATE TABLE CfgTemperatureUnit
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	Description TEXT,
	-- constraints
	UNIQUE (Description) ON CONFLICT ROLLBACK,
	CHECK(LENGTH(Description)>0) ON CONFLICT ROLLBACK
);

INSERT INTO CfgTemperatureUnit VALUES(1, '°F');

INSERT INTO CfgTemperatureUnit VALUES(2, '°C');

CREATE TABLE CfgCurrencyUnit
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	Description TEXT,
	-- constraints
	UNIQUE (Description) ON CONFLICT ROLLBACK,
	CHECK(LENGTH(Description)>0) ON CONFLICT ROLLBACK
);

INSERT INTO CfgCurrencyUnit VALUES(1, '$');

INSERT INTO CfgCurrencyUnit VALUES(2, '€');
