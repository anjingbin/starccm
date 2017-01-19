# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/psql/nls.mk,v 1.2 2004/07/05 08:55:54 SuLiang Exp $
CATALOG_NAME	:= psql
AVAIL_LANGUAGES	:= cs de fr ru sv zh_CN zh_TW
GETTEXT_FILES	:= command.c common.c copy.c help.c input.c large_obj.c \
                   mainloop.c print.c startup.c describe.c
GETTEXT_TRIGGERS:= _ psql_error simple_prompt
