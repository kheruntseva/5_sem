--1 �������
--docker exec -it e63a481a712e630be808dc6fb87f15b0c12d32411d006aca88528c6027ef9940 sqlplus sys/password@FREEPDB1 as sysdba

--SELECT name, open_mode FROM v$pdbs;  -- ����� PDB � ����� (READ WRITE, READ ONLY, MOUNTED � �.�.)
--SELECT pdb_name, status FROM cdb_pdbs ORDER BY pdb_id;
--2 �������

--SELECT instance_name, host_name, version FROM v$instance;

--3 �������

--SELECT comp_name, version, status FROM dba_registry;

---4 � 5 �������
--��� ����� ������ � ��� ������ ����� ������� ������, ����� �������� pdb ���� ������ ctrl+c, � ��� �� �����

--docker exec -it e63a481a712e630be808dc6fb87f15b0c12d32411d006aca88528c6027ef9940 bash

--��������� ���������
--export ORACLE_HOME=/opt/oracle/product/23ai/dbhomeFree
--export PATH=$ORACLE_HOME/bin:$PATH
--export LD_LIBRARY_PATH=$ORACLE_HOME/lib
--export ORACLE_SID=FREE

--������ �������� PDB
--sqlplus / as sysdba <<EOF
--ALTER PLUGGABLE DATABASE KDU_PDB CLOSE IMMEDIATE;
--DROP PLUGGABLE DATABASE KDU_PDB INCLUDING DATAFILES;
--CREATE PLUGGABLE DATABASE KDU_PDB ADMIN USER pdbadmin IDENTIFIED BY password
--  DEFAULT TABLESPACE users
--  FILE_NAME_CONVERT = ('/opt/oracle/oradata/FREE/pdbseed/', '/opt/oracle/oradata/FREE/KDU_PDB/');
--ALTER PLUGGABLE DATABASE KDU_PDB OPEN;
--ALTER SYSTEM REGISTER;
--SELECT name, open_mode FROM v$pdbs;
--SELECT pdb_name, status FROM cdb_pdbs ORDER BY pdb_id;
--EXIT;
--EOF

--�������� listener
--lsnrctl status

--���� ���������� ������
--ALTER SESSION SET CONTAINER=KDU_PDB;
--GRANT DBA TO pdbadmin;
--EXIT;