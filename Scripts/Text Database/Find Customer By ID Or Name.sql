SELECT	CustomerID,
	CustomerName
FROM	customers.csv
WHERE	CustomerID = ${Customer ID}
OR	CustomerName = '${Customer Name}'
