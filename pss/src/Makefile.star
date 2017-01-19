ALL_CXXFLAGS    = $(CXXFLAGS) -DStarBus
ALL_CPPFLAGS    = -I. $(PSS_CPPFLAGS) $(PSQL_CPPFLAGS) $(OTS_CPPFLAGS) $(OB_CPPFLAGS) $(CPPFLAGS)
ALL_IDLFLAGS    = -I$(IDLDIR) -I$(OTS_IDLDIR) $(OB_IDLFLAGS) -DStarBus --same-process \
                  --dll-import STAR_IMPORT
			  
