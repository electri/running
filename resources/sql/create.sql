PRAGMA user_version = 2;



CREATE TABLE ShoeMaker
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	Description TEXT,
	-- constraints
	UNIQUE (Description) ON CONFLICT ROLLBACK,
	CHECK(LENGTH(Description)>0) ON CONFLICT ROLLBACK
);



CREATE TABLE ShoeModel
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	Description TEXT,
	-- additional info
	ShoeMakerId INTEGER,
	-- constraints
	UNIQUE (ShoeMakerId,Description) ON CONFLICT ROLLBACK,
	CHECK(LENGTH(Description)>0) ON CONFLICT ROLLBACK
);

CREATE TRIGGER fki_ShoeModel_ShoeMaker_Id
BEFORE INSERT ON ShoeModel
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_ShoeModel_ShoeMaker_Id INSERT foreign key constraint failed')
	WHERE (SELECT Id FROM ShoeMaker WHERE Id = NEW.ShoeMakerId) IS NULL;
END;

CREATE TRIGGER fku_ShoeModel_ShoeMaker_Id
BEFORE UPDATE ON ShoeModel
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fku_ShoeModel_ShoeMaker_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM ShoeMaker WHERE Id = NEW.ShoeMakerId) IS NULL;
END;

CREATE TRIGGER fkd_ShoeModel_ShoeMaker_Id
BEFORE DELETE ON ShoeMaker
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fkd_ShoeModel_ShoeMaker_Id DELETE foreign key constraint failed')
	WHERE (SELECT ShoeMakerId FROM ShoeModel WHERE ShoeMakerId = OLD.Id) IS NOT NULL;
END;



CREATE TABLE Shoe
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	ShoeMakerId INTEGER,
	ShoeModelId INTEGER,
	Size REAL,
	PurchaseDate TEXT,
	Price REAL,
	InitialDistance REAL,
	Retired TEXT,
	Notes TEXT
);

CREATE TRIGGER fki_Shoe_ShoeMaker_Id
BEFORE INSERT ON Shoe
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Shoe_ShoeMaker_Id INSERT foreign key constraint failed')
	WHERE (SELECT Id FROM ShoeMaker WHERE Id = NEW.ShoeMakerId) IS NULL;
END;

CREATE TRIGGER fku_Shoe_ShoeMaker_Id
BEFORE UPDATE ON Shoe
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fku_Shoe_ShoeMaker_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM ShoeMaker WHERE Id = NEW.ShoeMakerId) IS NULL;
END;

CREATE TRIGGER fkd_Shoe_ShoeMaker_Id
BEFORE DELETE ON ShoeMaker
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fkd_Shoe_ShoeMaker_Id DELETE foreign key constraint failed')
	WHERE (SELECT ShoeMakerId FROM Shoe WHERE ShoeMakerId = OLD.Id) IS NOT NULL;
END;

CREATE TRIGGER fki_Shoe_ShoeModel_Id
BEFORE INSERT ON Shoe
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Shoe_ShoeModel_Id INSERT foreign key constraint failed')
	WHERE (SELECT Id FROM ShoeModel WHERE Id = NEW.ShoeModelId) IS NULL;
END;

CREATE TRIGGER fku_Shoe_ShoeModel_Id
BEFORE UPDATE ON Shoe
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fku_Shoe_ShoeModel_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM ShoeModel WHERE Id = NEW.ShoeModelId) IS NULL;
END;

CREATE TRIGGER fkd_Shoe_ShoeModel_Id
BEFORE DELETE ON ShoeModel
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fkd_Shoe_ShoeModel_Id DELETE foreign key constraint failed')
	WHERE (SELECT ShoeModelId FROM Shoe WHERE ShoeModelId = OLD.Id) IS NOT NULL;
END;



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



CREATE TABLE Event
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	Start DATE NOT NULL,
	Name TEXT,
	Description TEXT,
	EventTypeId INTEGER,
	Distance REAL,
	Duration TIME,
	Notes TEXT,
	ShoeId INTEGER,
	-- additional info
	Vote INTEGER,
	Quality INTEGER,
	Effort INTEGER,
	-- health
	Weight REAL,
	-- weather
	WeatherId INTEGER,
	Temperature REAL,
	-- constraints
	UNIQUE (Start) ON CONFLICT ROLLBACK
);

CREATE TRIGGER fki_Event_EventType_Id
BEFORE INSERT ON Event
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Event_EventType_Id INSERT foreign key constraint failed')
	WHERE (SELECT Id FROM EventType WHERE Id = NEW.EventTypeId) IS NULL;
END;

CREATE TRIGGER fku_Event_EventType_Id
BEFORE UPDATE ON Event
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fku_Event_EventType_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM EventType WHERE Id = NEW.EventTypeId) IS NULL;
END;

CREATE TRIGGER fkd_Event_EventType_Id
BEFORE DELETE ON EventType
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fkd_Event_EventType_Id DELETE foreign key constraint failed')
	WHERE (SELECT EventTypeId FROM Event WHERE EventTypeId = OLD.Id) IS NOT NULL;
END;

CREATE TRIGGER fkd_Event_Shoe_Id
BEFORE DELETE ON Shoe
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fkd_Event_Shoe_Id DELETE foreign key constraint failed')
	WHERE (SELECT ShoeId FROM Event WHERE ShoeId = OLD.Id) IS NOT NULL;
END;

CREATE TRIGGER fkd_Event_Weather_Id
BEFORE DELETE ON Weather
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fkd_Event_Weather_Id DELETE foreign key constraint failed')
	WHERE (SELECT WeatherId FROM Event WHERE WeatherId = OLD.Id) IS NOT NULL;
END;



CREATE TABLE IntervalType
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	Description TEXT,
	-- constraints
	UNIQUE (Description) ON CONFLICT ROLLBACK,
	CHECK(LENGTH(Description)>0) ON CONFLICT ROLLBACK
);



CREATE TABLE Interval
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- parent
	EventId INTEGER,
	-- table
	IntervalTypeId INTEGER,
	Distance REAL,
	Duration TIME,
	Notes TEXT
);

CREATE TRIGGER fki_Interval_Event_Id
BEFORE INSERT ON Interval
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Interval_Event_Id INSERT foreign key constraint failed')
	WHERE (SELECT Id FROM Event WHERE Id = NEW.EventId) IS NULL;
END;

CREATE TRIGGER fku_Interval_Event_Id
BEFORE UPDATE ON Interval
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fku_Interval_Event_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM IntervalType WHERE Id = NEW.EventId) IS NULL;
END;

CREATE TRIGGER fkd_Interval_Event_Id
BEFORE DELETE ON Event
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fkd_Interval_Event_Id DELETE foreign key constraint failed')
	WHERE (SELECT EventId FROM Interval WHERE EventId = OLD.Id) IS NOT NULL;
END;

CREATE TRIGGER fki_Interval_IntervalType_Id
BEFORE INSERT ON Interval
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Interval_IntervalType_Id INSERT foreign key constraint failed')
	WHERE (SELECT Id FROM IntervalType WHERE Id = NEW.IntervalTypeId) IS NULL;
END;

CREATE TRIGGER fku_Interval_IntervalType_Id
BEFORE UPDATE ON Interval
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fku_Interval_IntervalType_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM IntervalType WHERE Id = NEW.IntervalTypeId) IS NULL;
END;

CREATE TRIGGER fkd_Interval_IntervalType_Id
BEFORE DELETE ON IntervalType
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fkd_Interval_IntervalType_Id DELETE foreign key constraint failed')
	WHERE (SELECT IntervalTypeId FROM Interval WHERE IntervalTypeId = OLD.Id) IS NOT NULL;
END;



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



CREATE VIEW ShoeView AS
SELECT
	Shoe.Id AS Id,
	ShoeMaker.Description || ' ' || ShoeModel.Description AS Description,
	Shoe.Size AS Size, Shoe.PurchaseDate AS PurchaseDate,
	Shoe.Price AS Price, Shoe.InitialDistance AS InitialDistance,
	Shoe.Retired AS Retired, Shoe.Notes AS Notes
FROM Shoe
JOIN ShoeModel ON (ShoeModel.Id = Shoe.ShoeModelId)
JOIN ShoeMaker ON (ShoeMaker.Id = ShoeModel.ShoeMakerId);
