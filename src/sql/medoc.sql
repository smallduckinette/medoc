----------------------------------------------------
-- Copyright 2010 small_duck
-- 
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 2 of the License, or
-- (at your option) any later version.
-- 
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.
----------------------------------------------------

----------------------------------------------------
-- Langages
----------------------------------------------------
drop text search configuration if exists english_safe cascade;
create text search configuration english_safe ( copy = pg_catalog.english );
alter text search configuration english_safe drop mapping for int;
alter text search configuration english_safe drop mapping for uint;
alter text search configuration english_safe drop mapping for float;
alter text search configuration english_safe drop mapping for numword;

drop text search configuration if exists french_safe cascade;
create text search configuration french_safe ( copy = pg_catalog.french );
alter text search configuration french_safe drop mapping for int;
alter text search configuration french_safe drop mapping for uint;
alter text search configuration french_safe drop mapping for float;
alter text search configuration french_safe drop mapping for numword;

----------------------------------------------------
-- Tables
----------------------------------------------------
drop table if exists Account cascade;
create table Account(
	accoutId serial primary key, 
	login text not null unique,
	key text not null);
grant select on Account to medoc_user;

drop table if exists Document cascade;
create table Document(
	docid serial primary key,
	title bytea not null,
	date date not null,
	timestamp timestamp not null,
	isValid boolean not null);
grant select, insert, update, delete on Document to medoc_user;
grant select, update on Document_docid_seq to medoc_user;

drop table if exists Tag cascade;
create table Tag(
	docid integer not null references Document(docid),
	tag text not null,
	primary key(docid, tag));
grant select, insert, update, delete on Tag to medoc_user;

drop table if exists File cascade;
create table File(
	fileid serial primary key,
	docid integer references Document(docid),
	content tsvector not null,
	thumb bytea not null,
	file bytea not null,
	isValid boolean not null);
create index File_contents_idx on File using gin(content);
grant select, insert, update, delete on File to medoc_user;
grant select, update on File_fileid_seq to medoc_user;

----------------------------------------------------
-- Functions
----------------------------------------------------

-- Account management
drop function if exists createUser(text, text) cascade;
create function createUser(text, text) returns void as
$$
	insert into Account values(default, $1, crypt($2, gen_salt('bf')))
$$ language sql;

drop function if exists checkUser(text, text) cascade;
create function checkUser(text, text) returns boolean as
$$
	select count(*) = 1 from Account where login = $1 and crypt($2, key) = key;
$$ language sql;
grant execute on function checkUser(text, text) to medoc_user;

-- Get general info
drop function if exists getLanguages();
create function getLanguages() returns setof text as
$$
	select cfgname::text from pg_ts_config;
$$ language sql;
grant execute on function getLanguages() to medoc_user;

-- Document creation
-- createDocument(title, date, password)
drop function if exists createDocument(text, date, text);
create function createDocument(text, date, text) returns integer as
$$
	insert into Document values(default, pgp_sym_encrypt($1, $3), $2, now(), true) returning docid;
$$ language sql;
grant execute on function createDocument(text, date, text) to medoc_user;

-- tagDocument(docId, tag)
drop function if exists tagDocument(integer, text);
create function tagDocument(integer, text) returns void as
$$
	insert into Tag values($1, $2);
$$ language sql;
grant execute on function tagDocument(integer, text) to medoc_user;

-- addFile(docId, content, language, thumb, file, password)
drop function if exists addFile(integer, text, regconfig, bytea, bytea, text);
create function addFile(integer, text, regconfig, bytea, bytea, text) returns integer as
$$
	insert into File values(default, $1, to_tsvector($3, $2), pgp_sym_encrypt_bytea($4, $6), pgp_sym_encrypt_bytea($5, $6), true) returning fileid;
$$ language sql;
grant execute on function addFile(integer, text, regconfig, bytea, bytea, text) to medoc_user;

-- Document removal
drop function if exists deleteDocument(integer);
create function deleteDocument(integer) returns void as 
$$
	update Document set isValid = false where docid = $1;
$$ language sql;
grant execute on function deleteDocument(integer) to medoc_user;

-- Simple full text request
drop type if exists DocSearchT cascade;
create type DocSearchT as (docid integer, title text, date date);

drop function if exists searchFiles(text, text) cascade;
create function searchFiles(text, text) returns setof DocSearchT as
$$
	select d.docid, pgp_sym_decrypt(d.title, $2) as title, d.date
	from
		Document d
		join File f on f.docId = d.docId,
		to_tsquery($1) query
	where
		f.content @@ query
		and d.isValid = true
		and f.isValid = true
	group by
		d.docid, title, d.date
	order by
		max(ts_rank_cd(f.content, query)) desc	
$$ language sql;
grant execute on function searchFiles(text, text) to medoc_user;

drop function if exists searchFilesByLatest(int, text) cascade;
create function searchFilesByLatest(int, text) returns setof DocSearchT as
$$
	select d.docid, pgp_sym_decrypt(d.title, $2) as title, d.date
	from
		Document d
	where
		d.isValid = true
	order by
		d.docId desc
	limit $1;
$$ language sql;

-- Simple tag based search
drop function if exists searchFilesByTag(text, text) cascade;
create function searchFilesByTag(text, text) returns setof DocSearchT as
$$
	select d.docid, pgp_sym_decrypt(d.title, $2) as title, d.date
	from
		Document d
		join Tag t on t.docId = d.docId
	where
		t.tag = $1
		and d.isValid = true
	order by
		title;
$$ language sql;
grant execute on function searchFilesByTag(text, text) to medoc_user;

-- Document by id
drop function if exists getDocument(integer, text) cascade;
create function getDocument(integer, text) returns DocSearchT as
$$
	select d.docid, pgp_sym_decrypt(d.title, $2) as title, d.date
	from
		Document d
	where
		d.docid = $1
		and d.isValid = true;
$$ language sql;
grant execute on function getDocument(integer, text) to medoc_user;

-- Files for document
drop function if exists getFilesForDocument(integer) cascade;
create function getFilesForDocument(integer) returns setof integer as
$$
	select fileid from File where docid = $1 and isValid = true;
$$ language sql;
grant execute on function getFilesForDocument(integer) to medoc_user;

-- List of all tags
drop type if exists TagCount cascade;
create type TagCount as (tag text, count bigint);

drop function if exists getAllTags() cascade;
create function getAllTags() returns setof TagCount as
$$
	select tag, count(*)
	from
		Tag t
		join Document d on t.docId = d.docId
	where
		d.isValid = true
	group by tag
	order by tag;
$$ language sql;
grant execute on function getAllTags() to medoc_user;

-- Tags for document
drop function if exists getTagsForDocument(integer) cascade;
create function getTagsForDocument(integer) returns setof text as
$$
	select tag
	from
		Tag t
		join Document d on t.docId = d.docId
	where
		t.docid = $1
		and d.isValid = true
	order by
		tag;
$$ language sql;
grant execute on function getTagsForDocument(integer) to medoc_user;

-- Tags not belonging to document

drop function if exists getTagsNotForDocument(integer) cascade;
create function getTagsNotForDocument(integer) returns setof text as
$$
	select distinct t.tag
	from
		Tag t
		join Document d on t.docid = d.docid
	where
		d.isValid = true
		and t.tag not in (select t2.tag from Tag t2 join Document d2 on t2.docid = d2.docid where d2.docid = $1 and d2.isValid = true)
	order by t.tag;
$$ language sql;
grant execute on function getTagsNotForDocument(integer) to medoc_user;

-- Delete tag
drop function if exists deleteTag(integer, text) cascade;
create function deleteTag(integer, text) returns void as
$$
	delete from
		Tag
	where
		docid = $1
		and tag = $2
$$ language sql;
grant execute on function deleteTag(integer, text) to medoc_user;

-- Statistics

drop type if exists StatsT cascade;
create type StatsT as (docs bigint, files bigint, size text);

drop function if exists getStats() cascade;
create function getStats() returns StatsT as
$$
        select
                (select count(*) from document where isvalid = true),
                (select count(*) 
                 from file f join document d on f.docid = d.docid
                 where d.isvalid = true),
                (select pg_size_pretty(pg_database_size('medoc')))
$$ language sql;

