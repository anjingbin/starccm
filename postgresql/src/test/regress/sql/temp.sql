--
-- TEMP
-- Test temp relations and indexes
--

-- test temp table/index masking

CREATE TABLE temptest(col int);

CREATE INDEX i_temptest ON temptest(col);

CREATE TEMP TABLE temptest(col int);

CREATE INDEX i_temptest ON temptest(col);

DROP INDEX i_temptest;

DROP TABLE temptest;

DROP INDEX i_temptest;

DROP TABLE temptest;

-- test temp table selects

CREATE TABLE temptest(col int);

INSERT INTO temptest VALUES (1);

CREATE TEMP TABLE temptest(col int);

INSERT INTO temptest VALUES (2);

SELECT * FROM temptest;

DROP TABLE temptest;

SELECT * FROM temptest;

DROP TABLE temptest;

CREATE TEMP TABLE temptest(col int);

-- test temp table deletion

\c regression

SELECT * FROM temptest;

