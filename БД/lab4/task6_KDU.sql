-- Очистка таблицы
DELETE FROM KDU_table;
COMMIT;

-- PL/SQL для заполнения 10000 строк
BEGIN
  FOR i IN 1..10000 LOOP
    INSERT INTO KDU_table (id, name)
    VALUES (i, 'User_' || TO_CHAR(i));
  END LOOP;
  COMMIT;
END;
/

-- Проверка количества строк
SELECT COUNT(*) FROM KDU_table;