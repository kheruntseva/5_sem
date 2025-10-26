-- ������� �������
DELETE FROM KDU_table;
COMMIT;

-- PL/SQL ��� ���������� 10000 �����
BEGIN
  FOR i IN 1..10000 LOOP
    INSERT INTO KDU_table (id, name)
    VALUES (i, 'User_' || TO_CHAR(i));
  END LOOP;
  COMMIT;
END;
/

-- �������� ���������� �����
SELECT COUNT(*) FROM KDU_table;