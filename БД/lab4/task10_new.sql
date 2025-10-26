-- Подключение к CDB как SYS
SHOW CON_NAME; -- Должно быть CDB$ROOT

-- Список всех групп журналов повтора
SELECT group#, thread#, sequence#, bytes/1024/1024 AS size_mb, members, status
FROM v$log
ORDER BY group#;

-- Текущая группа журналов повтора
SELECT group#, status
FROM v$log
WHERE status = 'CURRENT';