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
	SELECT RAISE(ROLLBACK, 'fki_Interval_Event_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM IntervalType WHERE Id = NEW.EventId) IS NULL;
END;

CREATE TRIGGER fkd_Interval_Event_Id
BEFORE DELETE ON Event
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Interval_Event_Id DELETE foreign key constraint failed')
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
	SELECT RAISE(ROLLBACK, 'fki_Interval_IntervalType_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM IntervalType WHERE Id = NEW.IntervalTypeId) IS NULL;
END;

CREATE TRIGGER fkd_Interval_IntervalType_Id
BEFORE DELETE ON IntervalType
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Interval_IntervalType_Id DELETE foreign key constraint failed')
	WHERE (SELECT IntervalTypeId FROM Interval WHERE IntervalTypeId = OLD.Id) IS NOT NULL;
END;
