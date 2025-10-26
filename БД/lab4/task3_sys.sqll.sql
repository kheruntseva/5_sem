-- Список всех сегментов в KDU_QDATA
SELECT segment_name, segment_type, bytes/1024/1024 AS size_mb
FROM dba_segments
WHERE tablespace_name = 'KDU_QDATA'
ORDER BY segment_name;

-- Проверка сегмента KDU_table
SELECT segment_name, segment_type, bytes/1024/1024 AS size_mb
FROM dba_segments
WHERE tablespace_name = 'KDU_QDATA' AND segment_name = 'KDU_TABLE';

-- Остальные сегменты (кроме KDU_table)
SELECT segment_name, segment_type, bytes/1024/1024 AS size_mb
FROM dba_segments
WHERE tablespace_name = 'KDU_QDATA' AND segment_name != 'KDU_TABLE'
ORDER BY segment_name;