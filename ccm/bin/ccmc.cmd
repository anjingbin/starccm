rem @env.cmd
@setlocal
@set ORBDIR=E:\OB-4.1.0\ob
@set CCMDIR=E:\OB-CCM-1.0
@set includepath=-DORBacus -I. -I%ORBDIR%\idl -I%CCMDIR%\ccm\idl -I%CCMDIR%\pss\idl -I%CCMDIR%\ots\idl
rem @set includepath=-DStarBus -I. -I%ORBDIR%\idl -I%CCMDIR%\ccm\idl -I%CCMDIR%\pss\idl -I%CCMDIR%\star-ots\idl
idl3 %includepath% %~n1.idl3
@idl %includepath% %~n1.idl
cidl %includepath% --executor %~n1.cidl
@idl --no-skel %includepath% CCM_%~n1.idl
psdl %includepath% -DPSDL %~n1.psdl
rem star-psdl %includepath% -DPSDL %~n1.psdl
@endlocal