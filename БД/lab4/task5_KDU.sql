-- Подключение к FREEPDB1 как KDU
SHOW CON_NAME; -- Должно быть FREEPDB1

-- Восстановление таблицы
FLASHBACK TABLE KDU_table TO BEFORE DROP;

-- Проверка данных
SELECT * FROM KDU_table;

-- Проверка сегментов от SYS
-- (Переключитесь на Worksheet для FREEPDB1_SYS)
SELECT segment_name, segment_type, bytes/1024/1024 AS size_mb
FROM dba_segments
WHERE tablespace_name = 'KDU_QDATA' AND segment_name = 'KDU_TABLE';