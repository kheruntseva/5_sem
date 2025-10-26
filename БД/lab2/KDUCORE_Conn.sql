CREATE TABLE employees (
    emp_id NUMBER PRIMARY KEY,
    emp_name VARCHAR2(50),
    emp_salary NUMBER(10, 2)
);

CREATE VIEW high_paid_employees AS
SELECT emp_id, emp_name
FROM employees
WHERE emp_salary > 5000;

INSERT INTO employees VALUES (1, 'Anna Smith', 6000);
INSERT INTO employees VALUES (2, 'John Doe', 4500);
COMMIT;

SELECT * FROM high_paid_employees;

--11 команды в sqlplus:
--GRANT RL_KDUCORE TO KDUCORE;
-- GRANT CREATE TABLE TO KDUCORE;
--GRANT CREATE VIEW TO KDUCORE;
--ALTER USER KDUCORE QUOTA 10M ON TS_KDU;
--SQL> ALTER USER KDUCORE QUOTA UNLIMITED ON TS_KDU;
--docker exec -it e63a481a712e630be808dc6fb87f15b0c12d32411d006aca88528c6027ef9940 sqlplus sys/password@FREEPDB1 as sysdba
-- CREATE TABLESPACE XXX_QDATA DATAFILE 'XXX_QDATA.dbf' SIZE 10M OFFLINE;
-- ALTER TABLESPACE XXX_QDATA ONLINE;
-- ALTER USER KDUCORE QUOTA 2M ON XXX_QDATA;
-- docker exec -it e63a481a712e630be808dc6fb87f15b0c12d32411d006aca88528c6027ef9940 sqlplus KDUCORE/password@FREEPDB1
--    CREATE TABLE XXX_T1 (id NUMBER, description VARCHAR2(100)) TABLESPACE XXX_QDATA; 
--INSERT INTO XXX_T1 VALUES (1, 'First record');
--INSERT INTO XXX_T1 VALUES (2, 'Second record');
--INSERT INTO XXX_T1 VALUES (3, 'Third record');
--COMMIT;
--GRANT SELECT_CATALOG_ROLE TO KDUCORE;
--SELECT tablespace_name, status FROM dba_tablespaces WHERE tablespace_name = 'XXX_QDATA';
--SELECT tablespace_name, username, MAX_BYTES/1024/1024 AS quota_mbytes;
--SELECT * FROM XXX_T1;

