rem @env.cmd
@setlocal
@set ORBDIR=E:\StarBus5.0\StarBus5.0-Core\star
@set CCMDIR=E:\OB-CCM-1.0
rem @set includepath=-DORBacus -I. -I%ORBDIR%\idl -I%CCMDIR%\ccm\idl -I%CCMDIR%\pss\idl -I%CCMDIR%\ots\idl
@set includepath=-DStarBus -I. -I%ORBDIR%\idl -I%CCMDIR%\ccm\idl -I%CCMDIR%\pss\idl -I%CCMDIR%\star-ots\idl
idl3 %includepath% %~n1.idl3
@idl %includepath% %~n1.idl
cidl %includepath% %~n1.cidl
@idl --no-skel %includepath% CCM_%~n1.idl
rem psdl %includepath% -DPSDL %~n1.psdl
star-psdl %includepath% -DPSDL %~n1.psdl
@endlocal