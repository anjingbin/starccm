# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/pg_dump/nls.mk,v 1.2 2004/07/05 08:55:50 SuLiang Exp $
CATALOG_NAME	:= pg_dump
AVAIL_LANGUAGES	:= cs de ru sv zh_CN zh_TW
GETTEXT_FILES	:= pg_dump.c common.c pg_backup_archiver.c pg_backup_custom.c \
                   pg_backup_db.c pg_backup_files.c pg_backup_null.c \
                   pg_backup_tar.c pg_restore.c
GETTEXT_TRIGGERS:= write_msg:2 die_horribly:3 exit_horribly:3 simple_prompt \
                   ExecuteSqlCommand:3 ahlog:3
