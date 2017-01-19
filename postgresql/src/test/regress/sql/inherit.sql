--
-- Test inheritance features
--
CREATE TABLE a (aa TEXT);
CREATE TABLE b (bb TEXT) INHERITS (a);
CREATE TABLE c (cc TEXT) INHERITS (a);
CREATE TABLE d (dd TEXT) INHERITS (b,c,a);

INSERT INTO a(aa) VALUES('aaa');
INSERT INTO a(aa) VALUES('aaaa');
INSERT INTO a(aa) VALUES('aaaaa');
INSERT INTO a(aa) VALUES('aaaaaa');
INSERT INTO a(aa) VALUES('aaaaaaa');
INSERT INTO a(aa) VALUES('aaaaaaaa');

INSERT INTO b(aa) VALUES('bbb');
INSERT INTO b(aa) VALUES('bbbb');
INSERT INTO b(aa) VALUES('bbbbb');
INSERT INTO b(aa) VALUES('bbbbbb');
INSERT INTO b(aa) VALUES('bbbbbbb');
INSERT INTO b(aa) VALUES('bbbbbbbb');

INSERT INTO c(aa) VALUES('ccc');
INSERT INTO c(aa) VALUES('cccc');
INSERT INTO c(aa) VALUES('ccccc');
INSERT INTO c(aa) VALUES('cccccc');
INSERT INTO c(aa) VALUES('ccccccc');
INSERT INTO c(aa) VALUES('cccccccc');

INSERT INTO d(aa) VALUES('ddd');
INSERT INTO d(aa) VALUES('dddd');
INSERT INTO d(aa) VALUES('ddddd');
INSERT INTO d(aa) VALUES('dddddd');
INSERT INTO d(aa) VALUES('ddddddd');
INSERT INTO d(aa) VALUES('dddddddd');

SELECT relname, a.* FROM a, pg_class where a.tableoid = pg_class.oid;
SELECT relname, b.* FROM b, pg_class where b.tableoid = pg_class.oid;
SELECT relname, c.* FROM c, pg_class where c.tableoid = pg_class.oid;
SELECT relname, d.* FROM d, pg_class where d.tableoid = pg_class.oid;
SELECT relname, a.* FROM ONLY a, pg_class where a.tableoid = pg_class.oid;
SELECT relname, b.* FROM ONLY b, pg_class where b.tableoid = pg_class.oid;
SELECT relname, c.* FROM ONLY c, pg_class where c.tableoid = pg_class.oid;
SELECT relname, d.* FROM ONLY d, pg_class where d.tableoid = pg_class.oid;

UPDATE a SET aa='zzzz' WHERE aa='aaaa';
UPDATE ONLY a SET aa='zzzzz' WHERE aa='aaaaa';
UPDATE b SET aa='zzz' WHERE aa='aaa';
UPDATE ONLY b SET aa='zzz' WHERE aa='aaa';
UPDATE a SET aa='zzzzzz' WHERE aa LIKE 'aaa%';

SELECT relname, a.* FROM a, pg_class where a.tableoid = pg_class.oid;
SELECT relname, b.* FROM b, pg_class where b.tableoid = pg_class.oid;
SELECT relname, c.* FROM c, pg_class where c.tableoid = pg_class.oid;
SELECT relname, d.* FROM d, pg_class where d.tableoid = pg_class.oid;
SELECT relname, a.* FROM ONLY a, pg_class where a.tableoid = pg_class.oid;
SELECT relname, b.* FROM ONLY b, pg_class where b.tableoid = pg_class.oid;
SELECT relname, c.* FROM ONLY c, pg_class where c.tableoid = pg_class.oid;
SELECT relname, d.* FROM ONLY d, pg_class where d.tableoid = pg_class.oid;

UPDATE b SET aa='new';

SELECT relname, a.* FROM a, pg_class where a.tableoid = pg_class.oid;
SELECT relname, b.* FROM b, pg_class where b.tableoid = pg_class.oid;
SELECT relname, c.* FROM c, pg_class where c.tableoid = pg_class.oid;
SELECT relname, d.* FROM d, pg_class where d.tableoid = pg_class.oid;
SELECT relname, a.* FROM ONLY a, pg_class where a.tableoid = pg_class.oid;
SELECT relname, b.* FROM ONLY b, pg_class where b.tableoid = pg_class.oid;
SELECT relname, c.* FROM ONLY c, pg_class where c.tableoid = pg_class.oid;
SELECT relname, d.* FROM ONLY d, pg_class where d.tableoid = pg_class.oid;

UPDATE a SET aa='new';

DELETE FROM ONLY c WHERE aa='new';

SELECT relname, a.* FROM a, pg_class where a.tableoid = pg_class.oid;
SELECT relname, b.* FROM b, pg_class where b.tableoid = pg_class.oid;
SELECT relname, c.* FROM c, pg_class where c.tableoid = pg_class.oid;
SELECT relname, d.* FROM d, pg_class where d.tableoid = pg_class.oid;
SELECT relname, a.* FROM ONLY a, pg_class where a.tableoid = pg_class.oid;
SELECT relname, b.* FROM ONLY b, pg_class where b.tableoid = pg_class.oid;
SELECT relname, c.* FROM ONLY c, pg_class where c.tableoid = pg_class.oid;
SELECT relname, d.* FROM ONLY d, pg_class where d.tableoid = pg_class.oid;

DELETE FROM a;

SELECT relname, a.* FROM a, pg_class where a.tableoid = pg_class.oid;
SELECT relname, b.* FROM b, pg_class where b.tableoid = pg_class.oid;
SELECT relname, c.* FROM c, pg_class where c.tableoid = pg_class.oid;
SELECT relname, d.* FROM d, pg_class where d.tableoid = pg_class.oid;
SELECT relname, a.* FROM ONLY a, pg_class where a.tableoid = pg_class.oid;
SELECT relname, b.* FROM ONLY b, pg_class where b.tableoid = pg_class.oid;
SELECT relname, c.* FROM ONLY c, pg_class where c.tableoid = pg_class.oid;
SELECT relname, d.* FROM ONLY d, pg_class where d.tableoid = pg_class.oid;
