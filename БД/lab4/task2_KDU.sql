CREATE TABLE KDU_table (
  id NUMBER PRIMARY KEY,
  name VARCHAR2(50)
)
TABLESPACE KDU_QDATA;

INSERT INTO KDU_table (id, name) VALUES (1, 'Alice');
INSERT INTO KDU_table (id, name) VALUES (2, 'Bob');
INSERT INTO KDU_table (id, name) VALUES (3, 'Charlie');
COMMIT;

SELECT * FROM KDU_table;