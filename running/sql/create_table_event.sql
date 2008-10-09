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
	SELECT RAISE(ROLLBACK, 'fki_Event_EventType_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM EventType WHERE Id = NEW.EventTypeId) IS NULL;
END;

CREATE TRIGGER fkd_Event_EventType_Id
BEFORE DELETE ON EventType
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Event_EventType_Id DELETE foreign key constraint failed')
	WHERE (SELECT EventTypeId FROM Event WHERE EventTypeId = OLD.Id) IS NOT NULL;
END;


/*
CREATE TRIGGER fki_Event_Shoe_Id
BEFORE INSERT ON Event
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Event_Shoe_Id INSERT foreign key constraint failed')
	WHERE (SELECT Id FROM Shoe WHERE Id = NEW.ShoeId) IS NULL;
END;

CREATE TRIGGER fku_Event_Shoe_Id
BEFORE UPDATE ON Event
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Event_Shoe_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM Shoe WHERE Id = NEW.ShoeId) IS NULL;
END;
*/
CREATE TRIGGER fkd_Event_Shoe_Id
BEFORE DELETE ON Shoe
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Event_Shoe_Id DELETE foreign key constraint failed')
	WHERE (SELECT ShoeId FROM Event WHERE ShoeId = OLD.Id) IS NOT NULL;
END;


/*
CREATE TRIGGER fki_Event_Weather_Id
BEFORE INSERT ON Event
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Event_Weather_Id INSERT foreign key constraint failed')
	WHERE (SELECT Id FROM Weather WHERE Id = NEW.WeatherId) IS NULL;
END;

CREATE TRIGGER fku_Event_Weather_Id
BEFORE UPDATE ON Event
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Event_Weather_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM Weather WHERE Id = NEW.WeatherId) IS NULL;
END;
*/
CREATE TRIGGER fkd_Event_Weather_Id
BEFORE DELETE ON Weather
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Event_Weather_Id DELETE foreign key constraint failed')
	WHERE (SELECT WeatherId FROM Event WHERE WeatherId = OLD.Id) IS NOT NULL;
END;
