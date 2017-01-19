// **********************************************************************//
//                        STARCCM中OTS集成测试说明                       //
// **********************************************************************//

在本文件夹中是所有CCM应用服务器中关于集成OTS部分的测试。
在STARCCM_CMT目录中是所有的容器管理事务方式的测试。
在STARCCM_SMT目录中是所有的自管理事务方式的测试。


（1）STARCCM1.0 CMT方式测试：


在STARCCM_CMT文件夹中包括如下文件夹：
	src ------- 构件的源文件夹，其中放置了CMT方式下的构件实现文件和Client端实现文件。
	idl ------- 构件的IDL3 PSDL CIDL文件。
	bin ------- src中文件通过编译后生成的文件。

在src 文件夹中的文件说明：
	CCM_HelloWorld_impl.cpp ----- 构件实现源文件。
	CCM_HelloWorld_impl.h ------- 构件实现头文件。
	REQUIRES_NEW_Client.cpp ----- REQUIRES_NEW策略下构件的客户端源文件。
	MANDATORY_Client.cpp -------- MANDATORY策略下构件的客户端源文件。
	NEVER_Client.cpp ------------ NEVER策略下构件的客户端源文件。
	REQUIRED_Client.cpp --------- REQUIRED策略下构件的客户端源文件。
	NOT_SUPPORTED_Client.cpp ---- NOT_SUPPORTED策略下构件的客户端源文件。
	SUPPORTS_Client.cpp --------- SUPPORTS策略下构件的客户端源文件。


在进行CMT方式的测试时，在CCM的安装路径下需要修改的文件如下：
	1. HomeFinder_IP.txt是应用服务器运行的IP地址，在不同的运行环境中需要修改该文件中的IP地址，
	将其修改为应用服务器的IP。
	2. starccm.ini文件中包括两个部分：将main_host_ip 设置为应用服务器启动的IP地址，并设置host_ip地址，
	在本测试用例中将其设置的IP地址与main_host_ip 相同。
	3. 在Start_StarCCM_OTS_test.pl文件中用文本编辑器打开，在第101行中，在system("cmd /C Deploy -package 	REQUIRES_NEW_Package.zip &")中设置需要测试的用例，比如在此是REQUIRES_NEW_Package.zip，如果要测试其它用例就设置为
	其文件名。

在所有的XXX_Package文件夹中，是对于事务策略的构件包，在这些文件夹中包括一个*.cad文件，将其中的
IP地址替换为应用服务器运行的IP地址，再用标准的zip程序将hello.cad和person文件夹压缩打包。



运行测试用例的步骤：
	1.将HomeFinder_IP.txt和starccm.ini文件中的内容按上述该文件的功能进行修改；
	2.在所有的XXX_package文件夹中，是对于事务策略的构件包，在这些文件夹中包括一个*.cad文件，将其中的
	IP地址替换为应用服务器运行的IP地址，再用标准的zip程序将hello.cad和person文件夹压缩打包。	
	3.在Start_StarCCM_OTS_test.pl进行修改，修改方法在上面已经讲述,将其修改为所需测试的构件包的文件名；
	4.运行Start_StarCCM_OTS_test.pl，启动应用服务器，并部署在此文件中设置的构件包；
	5.运行与Start_StarCCM_OTS_test.pl中部署的构件包相对应的客户端可执行文件，比如：在Start_StarCCM_OTS_test.pl中部署的是		SUPPORTS_Package.zip，则运行SUPPORTS_Client.exe程序；
	6.测试完成后，需要先运行stop_server.pl，将应用服务器关闭，再进行另外的测试，需要从第1步重新开始。




（2）STARCCM1.0 SMT方式测试：


在STARCCM_SMT文件夹中包括如下文件夹：
	src ------- 构件的源文件夹，其中放置了SMT方式下的构件实现文件和Client端实现文件。
	idl ------- 构件的IDL3 PSDL CIDL文件。
	bin ------- src中文件通过编译后生成的文件。

在src 文件夹中的文件说明：
	CCM_HelloWorld_impl.cpp ----- 构件实现源文件。
	CCM_HelloWorld_impl.h ------- 构件实现头文件。
	SMT_Client.cpp -------------- self-managed策略下构件的客户端源文件。



运行测试用例的步骤：
	1.将HomeFinder_IP.txt和starccm.ini文件中的内容按上述该文件的功能进行修改；
	2.在SMT_Package.zip文件中，是对于事务策略的构件包，在这些文件夹中包括一个*.cad文件，将其中的
	IP地址替换为应用服务器运行的IP地址，再用标准的zip程序将hello.cad和person文件夹压缩打包；	
	3.在Start_StarCCM_OTS_test.pl进行修改，修改方法在上面已经讲述,将其修改为所需测试的构件包的文件名；
	4.运行Start_StarCCM_OTS_test.pl，启动应用服务器，并部署在此文件中设置的构件包；
	5.运行与Start_StarCCM_OTS_test.pl中部署的构件包相对应的客户端可执行文件，比如：在Start_StarCCM_OTS_test.pl中部署的是		SMT_Package.zip，则运行SMT_Client.exe程序；
	6.测试完成后，需要先运行stop_server.pl，将应用服务器关闭，再进行另外的测试，需要从第1步重新开始。
