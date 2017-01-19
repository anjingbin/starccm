/* $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/port/dynloader/darwin.h,v 1.1 2003/12/30 00:06:08 AnJingBin Exp $ */

#include "fmgr.h"

void	   *pg_dlopen(char *filename);
PGFunction	pg_dlsym(void *handle, char *funcname);
void		pg_dlclose(void *handle);
char	   *pg_dlerror(void);
