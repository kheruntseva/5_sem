-- Подключение к CDB как SYS
SHOW CON_NAME; -- Должно быть CDB$ROOT

-- Проверка текущей группы
SELECT group#, status
FROM v$log
WHERE status = 'CURRENT';

-- Переключение, если группа 4 текущая
ALTER SYSTEM SWITCH LOGFILE;

-- Удаление группы 4
ALTER DATABASE DROP LOGFILE GROUP 4;

-- Проверка удаления      
SELECT group#, thread#, sequence#, bytes/1024/1024 AS size_mb, members, status
FROM v$log
WHERE group# = 4;

SELECT group#, member
FROM v$logfile
WHERE group# = 4;