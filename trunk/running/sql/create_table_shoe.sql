CREATE TABLE Shoe
(
	-- header
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	-- table
	ShoeModelId INTEGER,
	Size REAL,
	PurchaseDate TEXT,
	Price REAL,
	InitialDistance REAL,
	Retired TEXT,
	Notes TEXT
);



CREATE TRIGGER fki_Shoe_ShoeModel_Id
BEFORE INSERT ON Shoe
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Shoe_ShoeModel_Id INSERT foreign key constraint failed')
	WHERE (SELECT Id FROM ShoeModel WHERE Id = NEW.ShoeModelId) IS NULL;
END;

CREATE TRIGGER fku_Shoe_ShoeModel_Id
BEFORE UPDATE ON Shoe
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Shoe_ShoeModel_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM ShoeModel WHERE Id = NEW.ShoeModelId) IS NULL;
END;

CREATE TRIGGER fkd_Shoe_ShoeModel_Id
BEFORE DELETE ON ShoeModel
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_Shoe_ShoeModel_Id DELETE foreign key constraint failed')
	WHERE (SELECT ShoeModelId FROM Shoe WHERE ShoeModelId = OLD.Id) IS NOT NULL;
END;
