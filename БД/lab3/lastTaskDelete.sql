-- ����������� � CDB ��� SYS
SHOW CON_NAME; -- ������ ���� CDB$ROOT

-- ������������ �� KDU_PDB ��� �������� ��������
ALTER SESSION SET CONTAINER=KDU_PDB;

-- �������� ������� KDU_table
DROP TABLE U1_KDU_PDB.KDU_table PURGE;

-- �������� ������������ U1_KDU_PDB
DROP USER U1_KDU_PDB CASCADE;

-- �������� ���� KDU_ROLE
DROP ROLE KDU_ROLE;

-- �������� ������� KDU_PROFILE
DROP PROFILE KDU_PROFILE CASCADE;

-- �������� ���������� ������������ KDU_DATA
DROP TABLESPACE KDU_DATA INCLUDING CONTENTS AND DATAFILES;

-- ������������ ������� �� CDB$ROOT
ALTER SESSION SET CONTAINER=CDB$ROOT;

-- �������� � �������� PDB KDU_PDB
--ALTER PLUGGABLE DATABASE KDU_PDB CLOSE IMMEDIATE;
--DROP PLUGGABLE DATABASE KDU_PDB INCLUDING DATAFILES;

-- �������� �������� PDB
SELECT name, open_mode FROM v$pdbs WHERE name = 'KDU_PDB';