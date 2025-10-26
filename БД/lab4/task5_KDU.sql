-- ����������� � FREEPDB1 ��� KDU
SHOW CON_NAME; -- ������ ���� FREEPDB1

-- �������������� �������
FLASHBACK TABLE KDU_table TO BEFORE DROP;

-- �������� ������
SELECT * FROM KDU_table;

-- �������� ��������� �� SYS
-- (������������� �� Worksheet ��� FREEPDB1_SYS)
SELECT segment_name, segment_type, bytes/1024/1024 AS size_mb
FROM dba_segments
WHERE tablespace_name = 'KDU_QDATA' AND segment_name = 'KDU_TABLE';