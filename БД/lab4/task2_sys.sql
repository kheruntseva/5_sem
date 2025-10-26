-- Подключение к FREEPDB1 как SYS
SHOW CON_NAME; -- Должно быть FREEPDB1

-- Создание табличного пространства KDU_QDATA (offline)
CREATE TABLESPACE KDU_QDATA
  DATAFILE '/opt/oracle/oradata/FREE/FREEPDB1/kdu_qdata01.dbf'
  SIZE 10M
  OFFLINE;

-- Перевод в online
ALTER TABLESPACE KDU_QDATA ONLINE;

-- Создание пользователя KDU
CREATE USER KDU IDENTIFIED BY kdu_password
  DEFAULT TABLESPACE KDU_QDATA
  TEMPORARY TABLESPACE TEMP
  QUOTA 2M ON KDU_QDATA;

-- Привилегии для создания таблиц и подключения
GRANT CREATE SESSION, CREATE TABLE TO KDU;

-- Создание таблицы KDU_table от имени KDU
-- (Выполняется в отдельном подключении KDU_FREEPDB1)
-- 

-- Проверка от SYS
SELECT tablespace_name, status FROM dba_tablespaces WHERE tablespace_name = 'KDU_QDATA';
SELECT username, default_tablespace FROM dba_users WHERE username = 'KDU';
SELECT table_name FROM dba_tables WHERE table_name = 'KDU_TABLE';

--УДАЛЕНИЕ ТОЛЬКО ПОСЛЕ ВСЕХ ЗАДАНИЙ БЛИН
-- Подключение к FREEPDB1 как SYS
SHOW CON_NAME; -- Должно быть FREEPDB1

DROP TABLE KDU.KDU_table PURGE;
DROP USER KDU CASCADE;
DROP TABLESPACE KDU_QDATA INCLUDING CONTENTS AND DATAFILES;

-- Проверка удаления
SELECT tablespace_name FROM dba_tablespaces WHERE tablespace_name = 'KDU_QDATA';
SELECT username FROM dba_users WHERE username = 'KDU';
SELECT table_name FROM dba_tables WHERE table_name = 'KDU_TABLE';