#!/usr/bin/perl

# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved
#                           
#  Author: Huang Jie	huangjie@email.com	
#          Xiao Heping  DoubleDragon_xhp@163.com
# **********************************************************************

system("ps | grep ComponentServer > pid.txt");
system("ps | grep Assembly >> pid.txt");
system("ps | grep FileAccessorFac >> pid.txt");
system("ps | grep ServerActivator >> pid.txt");
system("ps | grep AssemblyFactory >> pid.txt");
system("ps | grep ComponentInstal >> pid.txt");
system("ps | grep DeployDomainMgr >> pid.txt");
system("ps | grep DynDeploymentMg >> pid.txt");
system("ps | grep MainHostInstall >> pid.txt");
system("ps | grep HomeFinder >> pid.txt");
system("ps | grep nameserv >> pid.txt");
system("ps | grep ots >> pid.txt");
system("ps | grep -w res >> pid.txt");


if(open(SRC, "pid.txt"))
{
	while(<SRC>)
	{
		if(/([0-9]+)/)
		{
			system("kill $1");
		}
	}
	close(SRC);
}

sleep(1);

system("rm -rf pid.txt");
system("rm -rf *-*-*-*-* ");

#system("rm -rf Repository/*");
#system("rm -rf temp/*-*-*-*-* ");
#system("rm -rf temp/assembly ");
#system("rm -rf ttt");

system("rm -rf temp");
system("rm -rf Repository");

#system("rm -rf *.log");

system("rm -rf started");
system("rm -rf global_id001.dat");
system("rm -rf ApplicationServer.xml");

#system("rm -rf ccmhome");
#system("rm -rf hostname");
#system("rm -rf ip");
#system("rm -rf demo.ini");

system("rm -rf ots.ref");
system("rm -rf rf.ref");
system("rm -rf DomainConf.xml");
