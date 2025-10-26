------16

SHUTDOWN IMMEDIATE;
STARTUP MOUNT;

-- �������� �������� ������
SELECT log_mode FROM v$database;

-- ��������� �������������
ALTER DATABASE ARCHIVELOG;

ALTER DATABASE OPEN;

-- �������� ����� ���������
SELECT log_mode FROM v$database;

------17
-- �������� ������� �������� � �������
SELECT group#, sequence#, status
FROM v$log
ORDER BY group#;

SELECT sequence#, first_change#, next_change#
FROM v$archived_log
ORDER BY sequence# DESC;

-- �������������� ������������ ��� �������� ������
ALTER SYSTEM SWITCH LOGFILE;

-- ����������� ���������� ������
SELECT sequence# AS last_archive_number, name AS location, first_change# AS scn_start, next_change# AS scn_end
FROM v$archived_log
ORDER BY sequence# DESC
FETCH FIRST 1 ROWS ONLY;

-- �������� SCN � �������� � �������
SELECT group#, sequence#, first_change# AS scn_start, next_change# AS scn_end, status
FROM v$log
ORDER BY group#;

SELECT sequence#, first_change# AS scn_start, next_change# AS scn_end
FROM v$archived_log
ORDER BY sequence# DESC;

------18
-- �������� �������� ������
SELECT log_mode FROM v$database;

-- ���������� �������������
ALTER DATABASE NOARCHIVELOG;

-- �������� ����� ����������
SELECT log_mode FROM v$database;