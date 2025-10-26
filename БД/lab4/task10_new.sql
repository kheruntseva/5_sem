-- ����������� � CDB ��� SYS
SHOW CON_NAME; -- ������ ���� CDB$ROOT

-- ������ ���� ����� �������� �������
SELECT group#, thread#, sequence#, bytes/1024/1024 AS size_mb, members, status
FROM v$log
ORDER BY group#;

-- ������� ������ �������� �������
SELECT group#, status
FROM v$log
WHERE status = 'CURRENT';