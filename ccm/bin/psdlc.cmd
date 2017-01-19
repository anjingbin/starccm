@env.cmd
@setlocal
@set includepath=-DORBacus -I. -I%ORBDIR%\idl -I%CCMDIR%\idl\pss -I%CCMDIR%\idl\ots -I%CCMDIR%\idl\ccm
%PSDL% %includepath% %1
