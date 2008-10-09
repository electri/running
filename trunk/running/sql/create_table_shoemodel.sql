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
	SELECT RAISE(ROLLBACK, 'fki_ShoeModel_ShoeMaker_Id UPDATE foreign key constraint failed')
	WHERE (SELECT Id FROM ShoeMaker WHERE Id = NEW.ShoeMakerId) IS NULL;
END;

CREATE TRIGGER fkd_ShoeModel_ShoeMaker_Id
BEFORE DELETE ON ShoeMaker
FOR EACH ROW BEGIN
	SELECT RAISE(ROLLBACK, 'fki_ShoeModel_ShoeMaker_Id DELETE foreign key constraint failed')
	WHERE (SELECT ShoeMakerId FROM ShoeModel WHERE ShoeMakerId = OLD.Id) IS NOT NULL;
END;
