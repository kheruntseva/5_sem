SHOW CON_NAME;

-- Перманентные файлы данных
SELECT file_name, tablespace_name, bytes/1024/1024 AS size_mb, autoextensible
FROM dba_data_files
ORDER BY file_name;

-- Временные файлы
SELECT file_name, tablespace_name, bytes/1024/1024 AS size_mb, autoextensible
FROM dba_temp_files
ORDER BY file_name;