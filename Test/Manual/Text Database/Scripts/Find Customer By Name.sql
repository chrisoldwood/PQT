SELECT	CustomerID,
	CustomerName
FROM	customers.csv
WHERE	CustomerName like '%${Customer Name}%'
