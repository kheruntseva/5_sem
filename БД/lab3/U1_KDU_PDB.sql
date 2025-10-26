-- Создание таблицы KDU_table
CREATE TABLE KDU_table (
  id NUMBER PRIMARY KEY,
  name VARCHAR2(50),
  created_date DATE DEFAULT SYSDATE
)
TABLESPACE KDU_DATA;

-- Добавление строк
INSERT INTO KDU_table (id, name) VALUES (1, 'Alice');
INSERT INTO KDU_table (id, name) VALUES (2, 'Bob');
INSERT INTO KDU_table (id, name) VALUES (3, 'Charlie');

-- Коммит для сохранения данных
COMMIT;

-- SELECT-запрос
SELECT * FROM KDU_table;