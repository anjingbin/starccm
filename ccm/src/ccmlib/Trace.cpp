// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang Kebo    mep@263.net
// Author: Huang Jie    huangjie@email.com
// Author: Su Liang     centuryfree@yahoo.com.cn
//
// **********************************************************************

// Version: 1.0.0

#ifdef _DEBUG

#include <CORBA.h>
#include <stdlib.h>

#ifdef WIN32
#include <direct.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

int debugLevel = LEVEL6; 
bool debugAllModule = false;
bool withFileName = false;
bool withLineNumber = false;
bool debugLogFile = false;
char* debugModule = NULL;
char* noDebugModule = NULL;

FILE* logFileName = NULL;

volatile unsigned int lock_ = 1;

InitHelper dummy;

char* noPathFileName(char* file)
{
	char* tempName = file;
	char* noPathName = file;

#ifdef WIN32 
	while(tempName = strstr(tempName,"\\"))
#else
	while(tempName = strstr(tempName,"/"))
#endif
	{
		noPathName = ++tempName;
	}

	return noPathName;
}

bool checkDebugLevel(int lvl)
{
	if( lvl <= debugLevel ) 
		return true;
	else
		return false;
}

bool checkModule(const char* file)
{
#ifdef WIN32
	char* copy = _strlwr(_strdup((char*)file));
#else
	char* copy = strdup((char*)file);
#endif
	if(noDebugModule != NULL)
	{
#ifdef WIN32
		if(strstr(_strlwr(noDebugModule),copy))
#else
		if(strstr(noDebugModule,copy))
#endif
		{
			free(copy);
			return false; 
		}
	}

	if(debugAllModule)
	{
		free(copy);
		return true;
	}

	if(debugModule == NULL)
	{
		free(copy);
		return false;
	}
	
#ifdef WIN32
	if(strstr(_strlwr(debugModule),copy))
#else
	if(strstr(debugModule,copy))
#endif
	{
		free(copy);
		return true;
	}
	else
	{
		free(copy);
		return false; 
	}
}

InitHelper::InitHelper()
{
	const char* homeDir = getenv("CCM_HOME");
	
	if(homeDir != NULL && strcmp(homeDir, "") != 0)
	{
		chdir(homeDir);
	}
	else
	{
#ifdef WIN32
		chdir("c:\\ccm_home");
#else
		chdir("c:/ccm_home");
#endif
	}

	LoadProfile();

	if(debugLogFile)
	{
		char exeName[128];
		#ifdef WIN32
		GetModuleFileName(NULL, exeName, 128);
		char* index;
		char* old = exeName;
		while(index = strstr(old, "\\"))
		{
			old = index + 1;
		}
		strcpy(exeName, old);
		exeName[strlen(exeName)-4] = 0;

		DWORD id = GetCurrentProcessId();
		sprintf(exeName + strlen(exeName), "%d",id);
		strcat(exeName, ".log");
		#else
		strcpy(exeName, "trace.log");
		#endif
		logFileName = fopen(exeName,"w+");
	}
}

void 
InitHelper::LoadProfile()
{
	char rawBuf[1024];
	char buf[1024];
	char skip;
	char* value;
	char* oldBuf = rawBuf;
	char* newBuf = buf;

	FILE* iniFile = fopen("DebugConfig.ini","r");

	if(iniFile == NULL)
	{
		printf("Can't open `DebugConfig.ini\' file.\n");
		return;
	}

	while(!feof(iniFile))
	{
		oldBuf = rawBuf;
		
		fread(oldBuf++,sizeof(char),1,iniFile);

		while(*(oldBuf-1) != '\n' && !feof(iniFile))
		{
			if(*(oldBuf-1) == '\\')
			{
				oldBuf--;

				do
				{
					fread(&skip,sizeof(char),1,iniFile);
				} while(skip != '\n' && !feof(iniFile) );
			}

			fread(oldBuf++,sizeof(char),1,iniFile);
		}
		
		if(!feof(iniFile))
		{
			if(*(oldBuf-1) == '\n')
				*(oldBuf-1) = 0;
			else
				*oldBuf =0;
		}
		else
			*(oldBuf-1) = 0;

		oldBuf = rawBuf;
		newBuf = buf;

		while(*oldBuf)
		{
			if((*oldBuf) != ' ' && (*oldBuf) != '\t')
			{
				*newBuf = *oldBuf;
				newBuf ++;
				oldBuf ++;
			}
			else
				oldBuf ++;
		}

		*newBuf = 0;
		//printf("%s\n",buf);

		if(strncmp(buf,"DebugLevel",strlen("DebugLevel"))==0)
		{
			value = buf + strlen("DebugLevel") + 1;
			debugLevel = atoi(value);
		} 
		else if(strncmp(buf,"DebugAllModule",strlen("DebugAllModule"))==0)
		{
			value = buf + strlen("DebugAllModule") + 1;
			if(strcmp(value,"true") == 0)
				debugAllModule = true;
			else if(strcmp(value,"false") == 0)
				debugAllModule = false;
		}
		else if(strncmp(buf,"WithFileName",strlen("WithFileName"))==0)
		{
			value = buf + strlen("WithFileName") + 1;
			if(strcmp(value,"true") == 0)
				withFileName = true;
			else if(strcmp(value,"false") == 0)
				withFileName = false;
		}
		else if(strncmp(buf,"WithLineNumber",strlen("WithLineNumber"))==0)
		{
			value = buf + strlen("WithLineNumber") + 1;
			if(strcmp(value,"true") == 0)
				withLineNumber = true;
			else if(strcmp(value,"false") == 0)
				withLineNumber = false;
		}
		else if(strncmp(buf,"DebugLogFile",strlen("DebugLogFile"))==0)
		{
			value = buf + strlen("DebugLogFile") + 1;
			if(strcmp(value,"true") == 0)
				debugLogFile = true;
			else if(strcmp(value,"false") == 0)
				debugLogFile = false;
		}
		else if(strncmp(buf,"DebugModule",strlen("DebugModule"))==0)
		{
			value = buf + strlen("DebugModule") + 1;
			debugModule = strdup(value);
		}
		else if(strncmp(buf,"NoDebugModule",strlen("NoDebugModule"))==0)
		{
			value = buf + strlen("NoDebugModule") + 1;
			noDebugModule = strdup(value);
		}
	}

	fclose(iniFile);
}

#endif

