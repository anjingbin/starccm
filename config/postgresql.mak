#  **********************************************************************
#
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
#  Author: An Jingbin	anjb@sina.com	
# **********************************************************************


PSQL_HOME 	= $(top_srcdir)\..\postgresql
PSQL_INCLUDE 	= /I$(PSQL_HOME)\src\include /I$(PSQL_HOME)\src\interfaces\libpq
PSQL_LIBS	= $(PSQL_HOME)\src\interfaces\libpq\debug\libpq.lib