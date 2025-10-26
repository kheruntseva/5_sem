-- Подключение к CDB как SYS
SHOW CON_NAME; -- Должно быть CDB$ROOT

-- Текущее состояние групп
SELECT group#, sequence#, status
FROM v$log
ORDER BY group#;

-- Запись серверного времени первого переключения
SELECT TO_CHAR(SYSTIMESTAMP, 'YYYY-MM-DD HH24:MI:SS.FF TZR') AS first_switch_time
FROM dual;

-- Выполнение переключений (3 группы, полный цикл)
ALTER SYSTEM SWITCH LOGFILE;
ALTER SYSTEM SWITCH LOGFILE;
ALTER SYSTEM SWITCH LOGFILE;

-- Проверка состояния после цикла
SELECT group#, sequence#, status
FROM v$log
ORDER BY group#;
