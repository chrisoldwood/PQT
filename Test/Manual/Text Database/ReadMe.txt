Test Database [via DSN]
=======================

> %SystemRoot%\System32\odbcad32.exe

DSN Name:    PQT TEST
Description: CSV Test Database for PQT
Driver:      Microsoft Text Driver (*.txt;*.csv)
Directory:   ...\Test\Manual\Text Database\Database

Test Database [via Driver]
==========================

Driver:      Microsoft Access Text Driver (*.txt, *.csv)
File:        ;Dbq=...\Test\Manual\Text Database\Database

Note: this is a hack as it's not possible in PQT to explictly specify DBQ.

Test Query
==========

SELECT * FROM Customers.csv
