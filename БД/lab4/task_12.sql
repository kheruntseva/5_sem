-- ����������� � CDB ��� SYS
SHOW CON_NAME; -- ������ ���� CDB$ROOT

-- �������� ������� ����� � ������
SELECT group#, thread#, sequence#, bytes/1024/1024 AS size_mb, members, status
FROM v$log
ORDER BY group#;

SELECT group#, member
FROM v$logfile
ORDER BY group#;

-- �������� ����� ������ � 3 �������
ALTER DATABASE ADD LOGFILE GROUP 4 (
  '/opt/oracle/oradata/FREE/redo04a.log',
  '/opt/oracle/oradata/FREE/redo04b.log',
  '/opt/oracle/oradata/FREE/redo04c.log'
) SIZE 100M;

-- �������� ����� ������
SELECT group#, thread#, sequence#, bytes/1024/1024 AS size_mb, members, status
FROM v$log
WHERE group# = 4;

SELECT group#, member
FROM v$logfile
WHERE group# = 4;

-- ������������ �� ����� ������ � ������������ SCN
SELECT group#, sequence#, first_change# AS scn
FROM v$log
WHERE status = 'CURRENT';

ALTER SYSTEM SWITCH LOGFILE;

SELECT group#, sequence#, first_change# AS scn
FROM v$log
WHERE status = 'CURRENT';