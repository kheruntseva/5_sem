--1-2
-- Подключение к FREEPDB1 как SYS
SHOW CON_NAME; -- Должно быть FREEPDB1

DROP TABLE KDU.KDU_table PURGE;
DROP USER KDU CASCADE;
DROP TABLESPACE KDU_QDATA INCLUDING CONTENTS AND DATAFILES;

-- Проверка удаления
SELECT tablespace_name FROM dba_tablespaces WHERE tablespace_name = 'KDU_QDATA';
SELECT username FROM dba_users WHERE username = 'KDU';
SELECT table_name FROM dba_tables WHERE table_name = 'KDU_TABLE';

---------------------------------------------------------------

--3-5
-- Подключение к FREEPDB1 как SYS
SHOW CON_NAME; -- Должно быть FREEPDB1

-- Очистка корзины от имени KDU
-- (В Worksheet для KDU_FREEPDB1)
PURGE RECYCLEBIN;

-- Удаление таблицы
DROP TABLE KDU.KDU_table PURGE;

-- Удаление пользователя
DROP USER KDU CASCADE;

-- Удаление табличного пространства
DROP TABLESPACE KDU_QDATA INCLUDING CONTENTS AND DATAFILES;

-- Проверка удаления
SELECT tablespace_name FROM dba_tablespaces WHERE tablespace_name = 'KDU_QDATA';
SELECT username FROM dba_users WHERE username = 'KDU';
SELECT table_name FROM dba_tables WHERE table_name = 'KDU_TABLE';
SELECT object_name FROM user_recyclebin WHERE original_name = 'KDU_TABLE';

---------------------------------------------------------------

