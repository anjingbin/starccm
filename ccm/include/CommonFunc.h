// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Huang Jie    huangjie@email.com
//         Wang Kebo    mep@263.net
//         Su Liang     centuryfree@yahoo.com.cn
//
// **********************************************************************

// Version: 1.0.0

#ifndef __COMMONFUNC_H__
#define __COMMONFUNC_H__

namespace HelpFun
{
//	char * getHostIP();
	bool ObjectExist(CORBA::ORB_ptr orb_,const char *corbaloc,int timeout);
	int deleteDir(const char *dir);
	int copyFile(const char *source, const char *target);
	char * locateFile(const char *fileName);

	//added by xiao heping 2004/07/08
	char* GetCCMProfileString(const char* strSectionName,
								const char* keyName, 
								const char* defautValue,
								const char* fileName);
	//end added
}
#endif

