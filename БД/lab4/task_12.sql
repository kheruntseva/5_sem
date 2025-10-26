-- Подключение к CDB как SYS
SHOW CON_NAME; -- Должно быть CDB$ROOT

-- Проверка текущих групп и файлов
SELECT group#, thread#, sequence#, bytes/1024/1024 AS size_mb, members, status
FROM v$log
ORDER BY group#;

SELECT group#, member
FROM v$logfile
ORDER BY group#;

-- Создание новой группы с 3 файлами
ALTER DATABASE ADD LOGFILE GROUP 4 (
  '/opt/oracle/oradata/FREE/redo04a.log',
  '/opt/oracle/oradata/FREE/redo04b.log',
  '/opt/oracle/oradata/FREE/redo04c.log'
) SIZE 100M;

-- Проверка новой группы
SELECT group#, thread#, sequence#, bytes/1024/1024 AS size_mb, members, status
FROM v$log
WHERE group# = 4;

SELECT group#, member
FROM v$logfile
WHERE group# = 4;

-- Переключение на новую группу и отслеживание SCN
SELECT group#, sequence#, first_change# AS scn
FROM v$log
WHERE status = 'CURRENT';

ALTER SYSTEM SWITCH LOGFILE;

SELECT group#, sequence#, first_change# AS scn
FROM v$log
WHERE status = 'CURRENT';