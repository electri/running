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
