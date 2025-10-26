-- 1. �������� ���������� ������������
CREATE TABLESPACE KDU_DATA
  DATAFILE '/opt/oracle/oradata/FREE/KDU_PDB/kdu_data01.dbf'
  SIZE 100M
  AUTOEXTEND ON
  NEXT 10M
  MAXSIZE 500M;

-- 2. �������� ����
CREATE ROLE KDU_ROLE;
GRANT CREATE SESSION, CREATE TABLE, CREATE VIEW TO KDU_ROLE;

-- 3. �������� ������� ������������
CREATE PROFILE KDU_PROFILE LIMIT
  SESSIONS_PER_USER 5
  CPU_PER_SESSION 10000
  PASSWORD_LIFE_TIME 90
  FAILED_LOGIN_ATTEMPTS 3
  PASSWORD_LOCK_TIME 1;

-- 4. �������� ������������ U1_KDU_PDB
CREATE USER U1_KDU_PDB
  IDENTIFIED BY u1_password
  DEFAULT TABLESPACE KDU_DATA
  TEMPORARY TABLESPACE TEMP
  PROFILE KDU_PROFILE
  QUOTA UNLIMITED ON KDU_DATA;

-- ���������� ���� ������������
GRANT KDU_ROLE TO U1_KDU_PDB;

-- �������� ��������� ��������
SELECT tablespace_name FROM dba_tablespaces WHERE tablespace_name = 'KDU_DATA';
SELECT role FROM dba_roles WHERE role = 'KDU_ROLE';
SELECT profile FROM dba_profiles WHERE profile = 'KDU_PROFILE';
SELECT username FROM dba_users WHERE username = 'U1_KDU_PDB';