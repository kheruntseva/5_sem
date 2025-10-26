------16

SHUTDOWN IMMEDIATE;
STARTUP MOUNT;

-- Проверка текущего режима
SELECT log_mode FROM v$database;

-- Включение архивирования
ALTER DATABASE ARCHIVELOG;

ALTER DATABASE OPEN;

-- Проверка после включения
SELECT log_mode FROM v$database;

------17
-- Проверка текущих журналов и архивов
SELECT group#, sequence#, status
FROM v$log
ORDER BY group#;

SELECT sequence#, first_change#, next_change#
FROM v$archived_log
ORDER BY sequence# DESC;

-- Принудительное переключение для создания архива
ALTER SYSTEM SWITCH LOGFILE;

-- Определение последнего архива
SELECT sequence# AS last_archive_number, name AS location, first_change# AS scn_start, next_change# AS scn_end
FROM v$archived_log
ORDER BY sequence# DESC
FETCH FIRST 1 ROWS ONLY;

-- Проверка SCN в журналах и архивах
SELECT group#, sequence#, first_change# AS scn_start, next_change# AS scn_end, status
FROM v$log
ORDER BY group#;

SELECT sequence#, first_change# AS scn_start, next_change# AS scn_end
FROM v$archived_log
ORDER BY sequence# DESC;

------18
-- Проверка текущего режима
SELECT log_mode FROM v$database;

-- Отключение архивирования
ALTER DATABASE NOARCHIVELOG;

-- Проверка после отключения
SELECT log_mode FROM v$database;