PRAGMA user_version = 2;



CREATE TEMPORARY TABLE ShoeTEMP AS SELECT * FROM Shoe;

DROP TABLE Shoe;

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

CREATE TRIGGER fkd_Event_Shoe_Id
BEFORE DELETE ON Shoe
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fkd_Event_Shoe_Id DELETE foreign key constraint failed')
	WHERE (SELECT ShoeId FROM Event WHERE ShoeId = OLD.Id) IS NOT NULL;
END;

INSERT INTO Shoe (Id, ShoeMakerId, ShoeModelId, Size, PurchaseDate, Price, InitialDistance, Retired, Notes)
	SELECT ShoeTEMP.Id, ShoeModel.ShoeMakerId, ShoeTEMP.ShoeModelId, ShoeTEMP.Size, ShoeTEMP.PurchaseDate, ShoeTEMP.Price, ShoeTEMP.InitialDistance, ShoeTEMP.Retired, ShoeTEMP.Notes FROM ShoeTEMP JOIN ShoeModel ON ShoeModel.Id = ShoeTEMP.ShoeModelId;

DROP TABLE ShoeTEMP;



DROP TABLE Cfg;



VACUUM;
