$ENV{"runRoot"} = "..";
$ENV{"CLASSPATH"} = "$ENV{\"CLASSPATH\"};.;$ENV{\"runRoot\"}\\lib\\tools.jar;$ENV{\"runRoot\"}\\lib\\jbcl.jar;$ENV{\"runRoot\"}\\lib\\STAR.jar;$ENV{\"runRoot\"}\\classes";
system("start javaw -DCCM_HOME=$ENV{\"CCM_HOME\"}/ bank.Ui.AtmClient");

