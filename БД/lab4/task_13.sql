-- ����������� � CDB ��� SYS
SHOW CON_NAME; -- ������ ���� CDB$ROOT

-- �������� ������� ������
SELECT group#, status
FROM v$log
WHERE status = 'CURRENT';

-- ������������, ���� ������ 4 �������
ALTER SYSTEM SWITCH LOGFILE;

-- �������� ������ 4
ALTER DATABASE DROP LOGFILE GROUP 4;

-- �������� ��������      
SELECT group#, thread#, sequence#, bytes/1024/1024 AS size_mb, members, status
FROM v$log
WHERE group# = 4;

SELECT group#, member
FROM v$logfile
WHERE group# = 4;