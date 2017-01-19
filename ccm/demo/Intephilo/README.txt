1、关于这个例程
	这个例程演示了CORBA Component和EJB的集成。例程由经典的Philosopher改变而来。在这个例程中，原来的CCM构件Observer由EJB来实现；同时，Observer由使用事件通信改成了同步通信。

2、编译这个例程
	从命令行进入到demo/Intephilo目录，运行nmake /f Makefile.mak，编译得到CCM构件的动态链接库（位于demo/Intephilo目录）Dinner.dll和stdDinner.dll和一个EJB构件（位于demo/Intephilo\src\EJB\Dinner目录）Dinner-Observing.jar。

3、打包部署
	将CCM构件的动态链接库拷贝到demo/Intephilo/package相应的目录中，使用zip格式打包，即得到可部署的CCM构件包。而EJB的构件包拷贝到weblgoic的部署目录中即可（如D:\bea\weblogic81\samples\domains\examples\applications）。

4、运行
	首先需要修改CCM安装目录中的配置文件ConfigFile，将其中的Initial Service部分作相应的修改。其中，nis.orb.service.NameService表示CCM构件运行平台的名字服务引用；nis.orb.service.JNDI表示weblogic运行的JNDI的引用。一般只需修改主机名即可。对于weblogic运行平台，需要将EJB构件运行服务器的“协议=>IIOP=>缺省的宽字符集”改成utf-16，才能保证EJB在weblogic的控制台中的某些打印信息不是乱码。这些工作完成后，启动weblogic，待其启动完毕，再启动CCM运行平台，部署构件。