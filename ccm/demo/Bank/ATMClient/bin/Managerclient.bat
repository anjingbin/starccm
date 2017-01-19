set runRoot=..\

set ccmhomepath=.
set CLASSPATH=%classpath% .;%runRoot%\lib\tools.jar;%runRoot%\lib\jbcl.jar;%runRoot%\lib\STAR.jar;%runRoot%\classes
start javaw -DCCM_HOME=%ccmhomepath%/ bank.Ui.Client