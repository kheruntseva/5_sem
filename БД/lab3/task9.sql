SHOW CON_NAME;

-- Создание общего пользователя C##KDU
CREATE USER C##KDU IDENTIFIED BY c_kdu_password
  CONTAINER=ALL;

-- Привилегия для подключения к CDB
GRANT CREATE SESSION TO C##KDU;

-- Привилегия для подключения к KDU_PDB
ALTER SESSION SET CONTAINER=KDU_PDB;
GRANT CREATE SESSION TO C##KDU;
ALTER SESSION SET CONTAINER=CDB$ROOT;

-- Проверка пользователя
SELECT username, common FROM dba_users WHERE username = 'C##KDU';
SELECT grantee, privilege, admin_option FROM dba_sys_privs WHERE grantee = 'C##KDU';