set IdlFilePath=.
set includePath=-I. -I.\bin\idl\include
set componentName=Manager

jidl --no-skel --package manager --output-dir .\src --all %includePath% %componentName%.idl

