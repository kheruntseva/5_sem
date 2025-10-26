--удаление
DROP USER KDUCORE CASCADE;


DROP ROLE RL_KDUCORE;


DROP PROFILE PF_KDUCORE;


DROP TABLESPACE TS_KDU_TEMP INCLUDING CONTENTS AND DATAFILES;


DROP TABLESPACE TS_KDU INCLUDING CONTENTS AND DATAFILES;



--1 permanent tablespace
CREATE TABLESPACE TS_KDU
DATAFILE 'TS_KDU.dbf'
SIZE 7M
AUTOEXTEND ON NEXT 5M
MAXSIZE 20M;

--2 temporary tablespace
CREATE TEMPORARY TABLESPACE TS_KDU_TEMP
TEMPFILE 'TS_KDU_TEMP.dbf'
SIZE 5M
AUTOEXTEND ON NEXT 3M
MAXSIZE 30M;

--3 список tablespace
SELECT tablespace_name, status, contents
FROM dba_tablespaces;

--3 список файлов
SELECT file_name, tablespace_name, bytes/1024/1024 AS size_mb, autoextensible
FROM dba_data_files
UNION ALL
SELECT file_name, tablespace_name, bytes/1024/1024 AS size_mb, autoextensible
FROM dba_temp_files;

--4 роль с привилегиями
CREATE ROLE RL_KDUCORE;

GRANT CONNECT TO RL_KDUCORE;
GRANT CREATE TABLE, DROP ANY TABLE TO RL_KDUCORE;
GRANT CREATE VIEW, DROP ANY VIEW TO RL_KDUCORE;
GRANT CREATE PROCEDURE, DROP ANY PROCEDURE TO RL_KDUCORE;

--5 поиск роли в словаре
SELECT role
FROM dba_roles
WHERE role = 'RL_KDUCORE';

--5 получение всех привилегий, назначенных роли
SELECT privilege
FROM dba_sys_privs
WHERE grantee = 'RL_KDUCORE';

--6 профиль безопасности
CREATE PROFILE PF_KDUCORE LIMIT
  PASSWORD_LIFE_TIME 180
  SESSIONS_PER_USER 3
  FAILED_LOGIN_ATTEMPTS 7
  PASSWORD_LOCK_TIME 1
  PASSWORD_REUSE_TIME 10
  PASSWORD_GRACE_TIME DEFAULT
  CONNECT_TIME 180
  IDLE_TIME 30;

--7 список всех профилей
SELECT profile
FROM dba_profiles
GROUP BY profile;

--7 значения всех параметров PF_KDUCORE
SELECT resource_name, limit
FROM dba_profiles
WHERE profile = 'PF_KDUCORE';

--7 значения всех параметров профиля default
SELECT resource_name, limit
FROM dba_profiles
WHERE profile = 'DEFAULT';

--8 создание пользователя
CREATE USER KDUCORE
IDENTIFIED BY initial_password
DEFAULT TABLESPACE TS_KDU
TEMPORARY TABLESPACE TS_KDU_TEMP
PROFILE PF_KDUCORE
ACCOUNT UNLOCK
PASSWORD EXPIRE;

GRANT RL_KDUCORE TO KDUCORE;

GRANT CREATE SESSION TO KDUCORE;


-- Задание 9: Ручная смена пароля 
-- docker exec -it e63a481a712e630be808dc6fb87f15b0c12d32411d006aca88528c6027ef9940 sqlplus sys/password@FREEPDB1 as sysdba
-- ALTER USER KDUCORE IDENTIFIED BY password;
-- GRANT CREATE SESSION TO KDUCORE;
-- EXIT
-- docker exec -it e63a481a712e630be808dc6fb87f15b0c12d32411d006aca88528c6027ef9940 sqlplus KDUCORE/password@FREEPDB1

-- Задание 10: следщ файл, нужно зайти с созданного юзера
--ВАЖНО ПЕРЕД СЛЕД ЗАДАНИЕМ ВСЕ УДАЛИТЬ ЧТО ТАМ СОЗДАНО ОТ ЛИЦА НОВОГО ПОЛЬЗОВАТЕЛЯ:

DROP TABLE KDUCORE.employees;

DROP VIEW KDUCORE.high_paid_employees;

ALTER USER KDUCORE QUOTA 0 ON XXX_QDATA;

ALTER USER KDUCORE QUOTA UNLIMITED ON TS_KDU;

DROP TABLESPACE XXX_QDATA INCLUDING CONTENTS AND DATAFILES;