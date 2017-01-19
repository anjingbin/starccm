@env.cmd
@setlocal
@set includepath=-DORBacus -I. -I%ORBDIR%\idl -I%CCMDIR%\idl\pss -I%CCMDIR%\idl\ots -I%CCMDIR%\idl\ccm
idl3 %includepath% %1
@endlocal