-- ����������� � CDB ��� SYS
SHOW CON_NAME; -- ������ ���� CDB$ROOT

-- ������� ��������� �����
SELECT group#, sequence#, status
FROM v$log
ORDER BY group#;

-- ������ ���������� ������� ������� ������������
SELECT TO_CHAR(SYSTIMESTAMP, 'YYYY-MM-DD HH24:MI:SS.FF TZR') AS first_switch_time
FROM dual;

-- ���������� ������������ (3 ������, ������ ����)
ALTER SYSTEM SWITCH LOGFILE;
ALTER SYSTEM SWITCH LOGFILE;
ALTER SYSTEM SWITCH LOGFILE;

-- �������� ��������� ����� �����
SELECT group#, sequence#, status
FROM v$log
ORDER BY group#;
