-- ����������� � KDU_PDB ��� pdbadmin

-- 1. ��� ��������� ������������
SELECT tablespace_name, status, contents
FROM dba_tablespaces
ORDER BY tablespace_name;

-- 2. ������������ ����� ������
SELECT file_name, tablespace_name, bytes/1024/1024 AS size_mb, autoextensible
FROM dba_data_files
ORDER BY file_name;

-- 3. ��������� �����
SELECT file_name, tablespace_name, bytes/1024/1024 AS size_mb, autoextensible
FROM dba_temp_files
ORDER BY file_name;

-- 4. ��� ����
SELECT role
FROM dba_roles
ORDER BY role;

-- 5. ���������� ����� (��������� � ���������)
SELECT grantee AS role, privilege
FROM dba_sys_privs
WHERE grantee IN (SELECT role FROM dba_roles)
ORDER BY grantee, privilege;

SELECT grantee AS role, table_name, privilege
FROM dba_tab_privs
WHERE grantee IN (SELECT role FROM dba_roles)
ORDER BY grantee, table_name, privilege;

-- 6. ������� ������������
SELECT profile, resource_name, limit
FROM dba_profiles
ORDER BY profile, resource_name;

-- 7. ��� ������������ � KDU_PDB
SELECT username, default_tablespace, profile
FROM dba_users
ORDER BY username;

-- 8. ����������� ���� �������������
SELECT grantee AS username, granted_role
FROM dba_role_privs
WHERE grantee IN (SELECT username FROM dba_users)
ORDER BY grantee, granted_role;