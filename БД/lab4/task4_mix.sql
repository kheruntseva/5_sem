-- ����������� � FREEPDB1 ��� KDU
-- (����������� � Worksheet ��� KDU_FREEPDB1)
SHOW CON_NAME; -- ������ ���� FREEPDB1

-- �������� ������� KDU_table
DROP TABLE KDU_table;

-- ����������� � FREEPDB1 ��� SYS
-- (������������� �� Worksheet ��� FREEPDB1_SYS)
SHOW CON_NAME; -- ������ ���� FREEPDB1

-- ������ ��������� ����� ��������
SELECT segment_name, segment_type, bytes/1024/1024 AS size_mb
FROM dba_segments
WHERE tablespace_name = 'KDU_QDATA'
ORDER BY segment_name;

-- �������� �������� KDU_table
SELECT segment_name, segment_type, bytes/1024/1024 AS size_mb
FROM dba_segments
WHERE tablespace_name = 'KDU_QDATA' AND segment_name = 'KDU_TABLE';

-- �������� ������� �� ����� KDU
-- (������������� �� Worksheet ��� KDU_FREEPDB1)
SELECT object_name, original_name, type, can_undrop
FROM user_recyclebin
WHERE original_name = 'KDU_TABLE';