#!/bin/perl

# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved
#                           
#  Author: Huang Jie	huangjie@email.com	
# **********************************************************************

system("rm -f *.log");

if(-d "Repository")
{
}
else
{
	unlink("Repository");
	system("mkdir Repository");
}

if(-d "temp")
{
}
else
{
	unlink("temp");
	system("mkdir temp");
}

system("cp ApplicationServer.xml.xml ApplicationServer.xml");
#system("cd > ccmhome");
#
#open(SRC, "ccmhome") || die "Can't open ccmhome\n";
#$ccmhome = <SRC>;
#chomp($ccmhome);
#$configfile = $ccmhome."/ConfigFile";
#$ENV{"CCM_HOME"} = $ccmhome;
#print "CCM_HOME = ${ccmhome}\n";
#close(SRC);

if(open(SRC,"started"))
{
	system("stop_server.pl");
	close(SRC);
	sleep(3);
}

if(open(SRC,"starccm.properties"))
{
	while(<SRC>)
	{
		if(/[\s]*HomeFinderPort[\s]*=[\s]*([0-9]+)/g)
		{
			$HomeFinderPort = $1;
			next;
		}
		if(/[\s]*NameServicePort[\s]*=[\s]*([0-9]+)/g)
		{
			$NameServicePort = $1;
			next;
		}
		if(/[\s]*OtsPort[\s]*=[\s]*([0-9]+)/g)
		{
			$OtsPort = $1;
			next;
		}
		if(/[\s]*ResPort[\s]*=[\s]*([0-9]+)/g)
		{
			$ResPort = $1;
			next;
		}
#		if(/[\s]*ApplicationPackage[\s]*=[\s]*([^\s]*)/g)
#		{
#			$ccmPackage = $1;
#			next;
#		}
				
	}
	close(SRC);
}
#if($ccmPackage eq "PetStore.zip")
#{
#	GetPetStoreDatabaseConfig();
#	if($dbip eq "")
#	{
#		print "Sorry,你还没有配置pss数据库,程序无法运行!\n";
#		print "请用文本编辑器打开starccm.properties配置文件在[PetStore]下正确配置数据库。";
#	}
#	ChangePetStoreDatabaseConfig();
#}

system("echo started > started");
system("start /B nameserv -OAport ${NameServicePort} &");
system("start /B HomeFinder -OAport ${HomeFinderPort} &");
system("start ots -OAport ${OtsPort} &");
system("start /B res -OAport ${ResPort} &");

sleep(1);
system("cmd /C MainHostInstall &");
#sleep(2);
#system("cmd /C Deploy -package ${ccmPackage} &");


#sub GetPetStoreDatabaseConfig()
#{
#	if(open(SRC,"starccm.properties"))
#	{
#		while(<SRC>)
#		{
#			if(/[\s]*PetStore_Pss_Database_Host[\s]*=[\s]*([^\s]*)/g)
#			{
#				$dbip = $1;
#				next;
#			}
#			if(/[\s]*PetStore_Pss_Database_Name[\s]*=[\s]*([^\s]*)/g)
#			{
#				$database = $1;
#				next;
#			}
#			if(/[\s]*PetStore_Pss_Database_User[\s]*=[\s]*([^\s]*)/g)
#			{
#				$user = $1;
#				next;
#			}
#
#			if(/[\s]*PetStore_Pss_Database_Password[\s]*=[\s]*([^\s]*)/g)
#			{
#				$password = $1;
#				next;
#			}
#					
#		}
#		close(SRC);
#	}
#}
#sub ChangePetStoreDatabaseConfig()
#{
#	system("unzip.exe -o -d ttt PetStore.zip");
#	$cadfile = "PetStore.cad";
#	
#	print "Change ip address in $cadfile ...\n";
#	open(SRC, "ttt/${cadfile}") || die "Can't open ttt/${cadfile}\n";
#	open(DES, ">$cadfile") || die "Can't open ${cadfile}\n";
#	while(<SRC>)
#	{
#		if(/createresourcepool/)
#		{
#			s/host=\"[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+\"/host=\"${dbip}\"/g;
#			s/database="[^\s]+"/database="${database}"/g;
#			s/user="[^\s]+"/user="${user}"/g;
#			s/password="[^\s]+"/password="${password}"/g;
#		}
#					
#		print DES $_;
#	}
#	close(SRC);
#	close(DES);
#
#	system("zip.exe PetStore.zip ${cadfile}");
#
#	system("rm -rf *.cad");
#	system("rm -rf ttt");
#	
#}
