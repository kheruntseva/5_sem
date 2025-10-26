-- Подключение к CDB как SYS
SHOW CON_NAME; -- Должно быть CDB$ROOT

-- Переключение на KDU_PDB для удаления объектов
ALTER SESSION SET CONTAINER=KDU_PDB;

-- Удаление таблицы KDU_table
DROP TABLE U1_KDU_PDB.KDU_table PURGE;

-- Удаление пользователя U1_KDU_PDB
DROP USER U1_KDU_PDB CASCADE;

-- Удаление роли KDU_ROLE
DROP ROLE KDU_ROLE;

-- Удаление профиля KDU_PROFILE
DROP PROFILE KDU_PROFILE CASCADE;

-- Удаление табличного пространства KDU_DATA
DROP TABLESPACE KDU_DATA INCLUDING CONTENTS AND DATAFILES;

-- Переключение обратно на CDB$ROOT
ALTER SESSION SET CONTAINER=CDB$ROOT;

-- Закрытие и удаление PDB KDU_PDB
--ALTER PLUGGABLE DATABASE KDU_PDB CLOSE IMMEDIATE;
--DROP PLUGGABLE DATABASE KDU_PDB INCLUDING DATAFILES;

-- Проверка удаления PDB
SELECT name, open_mode FROM v$pdbs WHERE name = 'KDU_PDB';