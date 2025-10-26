-- Подключение к FREEPDB1 как SYS
SHOW CON_NAME; -- Должно быть FREEPDB1

-- Количество экстентов и их размер
SELECT segment_name, COUNT(*) AS extent_count, 
       SUM(blocks) AS total_blocks, 
       SUM(bytes) AS total_bytes
FROM dba_extents
WHERE segment_name = 'KDU_TABLE' AND owner = 'KDU' AND tablespace_name = 'KDU_QDATA'
GROUP BY segment_name;

-- Список всех экстентов
SELECT extent_id, file_id, block_id, blocks, bytes
FROM dba_extents
WHERE segment_name = 'KDU_TABLE' AND owner = 'KDU' AND tablespace_name = 'KDU_QDATA'
ORDER BY extent_id;