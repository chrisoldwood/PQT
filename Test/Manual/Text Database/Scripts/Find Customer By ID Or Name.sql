SELECT	CustomerID,
	CustomerName
FROM	customers.csv
WHERE	CustomerID = ${Customer ID}
OR	CustomerName LIKE '%${Customer Name}%'
