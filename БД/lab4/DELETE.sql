--1-2
-- ����������� � FREEPDB1 ��� SYS
SHOW CON_NAME; -- ������ ���� FREEPDB1

DROP TABLE KDU.KDU_table PURGE;
DROP USER KDU CASCADE;
DROP TABLESPACE KDU_QDATA INCLUDING CONTENTS AND DATAFILES;

-- �������� ��������
SELECT tablespace_name FROM dba_tablespaces WHERE tablespace_name = 'KDU_QDATA';
SELECT username FROM dba_users WHERE username = 'KDU';
SELECT table_name FROM dba_tables WHERE table_name = 'KDU_TABLE';

---------------------------------------------------------------

--3-5
-- ����������� � FREEPDB1 ��� SYS
SHOW CON_NAME; -- ������ ���� FREEPDB1

-- ������� ������� �� ����� KDU
-- (� Worksheet ��� KDU_FREEPDB1)
PURGE RECYCLEBIN;

-- �������� �������
DROP TABLE KDU.KDU_table PURGE;

-- �������� ������������
DROP USER KDU CASCADE;

-- �������� ���������� ������������
DROP TABLESPACE KDU_QDATA INCLUDING CONTENTS AND DATAFILES;

-- �������� ��������
SELECT tablespace_name FROM dba_tablespaces WHERE tablespace_name = 'KDU_QDATA';
SELECT username FROM dba_users WHERE username = 'KDU';
SELECT table_name FROM dba_tables WHERE table_name = 'KDU_TABLE';
SELECT object_name FROM user_recyclebin WHERE original_name = 'KDU_TABLE';

---------------------------------------------------------------

