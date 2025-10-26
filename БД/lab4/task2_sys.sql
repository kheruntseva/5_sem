-- ����������� � FREEPDB1 ��� SYS
SHOW CON_NAME; -- ������ ���� FREEPDB1

-- �������� ���������� ������������ KDU_QDATA (offline)
CREATE TABLESPACE KDU_QDATA
  DATAFILE '/opt/oracle/oradata/FREE/FREEPDB1/kdu_qdata01.dbf'
  SIZE 10M
  OFFLINE;

-- ������� � online
ALTER TABLESPACE KDU_QDATA ONLINE;

-- �������� ������������ KDU
CREATE USER KDU IDENTIFIED BY kdu_password
  DEFAULT TABLESPACE KDU_QDATA
  TEMPORARY TABLESPACE TEMP
  QUOTA 2M ON KDU_QDATA;

-- ���������� ��� �������� ������ � �����������
GRANT CREATE SESSION, CREATE TABLE TO KDU;

-- �������� ������� KDU_table �� ����� KDU
-- (����������� � ��������� ����������� KDU_FREEPDB1)
-- 

-- �������� �� SYS
SELECT tablespace_name, status FROM dba_tablespaces WHERE tablespace_name = 'KDU_QDATA';
SELECT username, default_tablespace FROM dba_users WHERE username = 'KDU';
SELECT table_name FROM dba_tables WHERE table_name = 'KDU_TABLE';

--�������� ������ ����� ���� ������� ����
-- ����������� � FREEPDB1 ��� SYS
SHOW CON_NAME; -- ������ ���� FREEPDB1

DROP TABLE KDU.KDU_table PURGE;
DROP USER KDU CASCADE;
DROP TABLESPACE KDU_QDATA INCLUDING CONTENTS AND DATAFILES;

-- �������� ��������
SELECT tablespace_name FROM dba_tablespaces WHERE tablespace_name = 'KDU_QDATA';
SELECT username FROM dba_users WHERE username = 'KDU';
SELECT table_name FROM dba_tables WHERE table_name = 'KDU_TABLE';