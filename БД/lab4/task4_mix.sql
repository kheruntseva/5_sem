-- Подключение к FREEPDB1 как KDU
-- (Выполняется в Worksheet для KDU_FREEPDB1)
SHOW CON_NAME; -- Должно быть FREEPDB1

-- Удаление таблицы KDU_table
DROP TABLE KDU_table;

-- Подключение к FREEPDB1 как SYS
-- (Переключитесь на Worksheet для FREEPDB1_SYS)
SHOW CON_NAME; -- Должно быть FREEPDB1

-- Список сегментов после удаления
SELECT segment_name, segment_type, bytes/1024/1024 AS size_mb
FROM dba_segments
WHERE tablespace_name = 'KDU_QDATA'
ORDER BY segment_name;

-- Проверка сегмента KDU_table
SELECT segment_name, segment_type, bytes/1024/1024 AS size_mb
FROM dba_segments
WHERE tablespace_name = 'KDU_QDATA' AND segment_name = 'KDU_TABLE';

-- Проверка корзины от имени KDU
-- (Переключитесь на Worksheet для KDU_FREEPDB1)
SELECT object_name, original_name, type, can_undrop
FROM user_recyclebin
WHERE original_name = 'KDU_TABLE';