@env.cmd
@setlocal
@set includepath=-DStarBus -I. -I%ORBDIR%\idl -I%CCMDIR%\idl\pss -I%CCMDIR%\idl\ots -I%CCMDIR%\idl\ccm
idl %includepath% %1
@endlocal