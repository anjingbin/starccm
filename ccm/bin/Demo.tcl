#!/bin/sh
# the next line restarts using wish \
exec wish "$0" "$@"

eval destroy [winfo child .]
wm title . "CCM Demo Controler"
#set widgetDemo 1

set font {Helvetica 14}
menu .menuBar -tearoff 0
.menuBar add cascade -menu .menuBar.file -label "File" -underline 0
menu .menuBar.file -tearoff 0


.menuBar.file add command -label "About..." -command "aboutBox" \
-underline 0 -accelerator "<F1>"
.menuBar.file add sep

.menuBar.file add command -label "Quit" -command "exit" -underline 0 
. configure -menu .menuBar 
bind . <F1> aboutBox

frame .statusBar
label .statusBar.lab -text "   " -relief sunken -bd 1 \
    -font -*-Helvetica-Medium-R-Normal--*-120-*-*-*-*-*-* -anchor w
label .statusBar.foo -width 8 -relief sunken -bd 1 \
    -font -*-Helvetica-Medium-R-Normal--*-120-*-*-*-*-*-* -anchor w
pack .statusBar.lab -side left -padx 2 -expand yes -fill both
pack .statusBar.foo -side left -padx 2
pack .statusBar -side bottom -fill x -pady 2

button .migrateButton  -text "migrate package here according ip address below" -width 80 \
    -command "migrate"
pack .migrateButton -side top

button .initDBButton  -text "Init DB" -width 80 \
    -command "initDB"
pack .initDBButton -side top

#button .otsButton  -text "Start OTS" -width 80 \
#    -command "startOTS"
#pack .otsButton -side top

button .startFacilityButton  -text "Start Name Service and HomeFinder" -width 80 \
    -command "startFacility"
pack .startFacilityButton -side top

#button .starConfiguratorButton  -text "start Pet Store configurator" -width 80 \
#    -command "exec Configurator.exe -OAport 9993 &"
#pack .starConfiguratorButton -side top

button .starDomainManagerButton  -text "Start Domain Manager" -width 80 \
    -command "exec cmd /C MainHostInstall.exe &"
pack .starDomainManagerButton -side top

set fd [open "demotcl.ini" r]
fconfigure $fd -buffering line
gets $fd data
set ip $data
gets $fd data
set package $data
close $fd

frame .frame1
pack .frame1 -side top 

label .frame1.ipAddressLable -text "Please input domain manager's ip:"
entry .frame1.ipAddressEntry -width 48
.frame1.ipAddressEntry insert 0 $ip
pack .frame1.ipAddressLable .frame1.ipAddressEntry -side left

frame .frame2
pack .frame2 -side top 

label .frame2.ipPackageLable -text "Please input package's location and name:"
entry .frame2.ipPackageEntry -width 40
.frame2.ipPackageEntry insert 0 $package
pack .frame2.ipPackageLable .frame2.ipPackageEntry -side left

button .setButton  -text "set default value:" -width 80 \
    -command "setDefaultValue"
pack .setButton -side top

button .deployButton  -text "Start Deploy" -width 80 \
    -command "startDeploy"
pack .deployButton -side top

button .stopButton  -text "Stop and Clean temporary files" -width 80 \
    -command "setStop"
pack .stopButton -side top

proc migrate {} {
	setDefaultValue
	set package [ .frame2.ipPackageEntry get ]
	set ip [ .frame1.ipAddressEntry get ]
	exec unzip.exe -o -d ttt $package
	set cadfiles [glob ttt/*.cad]
	foreach cad $cadfiles {
		set rfd [open $cad r]
		set wfd [open ttt/temp.txt w]

		while {![eof $rfd]} {
			set line [gets $rfd] 
			regsub {[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+} $line $ip line 
			puts $wfd $line
		}

		close $rfd
		close $wfd
		exec mv.exe -f ttt/temp.txt $cad
	}
	exec cp.exe ttt/*.cad .
	exec rm.exe -rf ttt
	exec zip.exe $package *.cad
	exec rm.exe -rf *.cad
}

proc startFacility {} {
	setDefaultValue
	exec OTS.exe -OAport 9995 &
	exec RES.exe -OAport 9996 &
	exec nameserv.exe -OAport 9991 &
	exec HomeFinder.exe -OAport 9992 &
}

proc startOTS {} {
	setDefaultValue
	exec OTS.exe -OAport 9995 &
	exec RES.exe -OAport 9996 &
}

proc initDB {} {
	setDefaultValue
	exec expect.exe -f initDB.expect
}

proc startDeploy {} {
	setDefaultValue
	exec Deploy.exe < demotcl.ini &
}

proc setDefaultValue {} {
	set fd [open "demotcl.ini" w]
	fconfigure $fd -buffering line
	set ip [ .frame1.ipAddressEntry get ]
	puts $fd $ip
	set package [ .frame2.ipPackageEntry get ]
	puts $fd $package
	close $fd    
}

proc setStop {} {
	setDefaultValue	
	#exec expect.exe -f closedb.expect	
	catch {exec ps.exe | grep.exe ComponentServer > pid.txt}
	catch {exec ps.exe | grep.exe Assembly >> pid.txt}
	catch {exec ps.exe | grep.exe FileAccessorFactory >> pid.txt}
	catch {exec ps.exe | grep.exe ServerActivator >> pid.txt}
	catch {exec ps.exe | grep.exe AssemblyFactory >> pid.txt}
	catch {exec ps.exe | grep.exe ComponentInstallation >> pid.txt}
	catch {exec ps.exe | grep.exe DeployDomainMgr >> pid.txt}
	catch {exec ps.exe | grep.exe MainHostInstall >> pid.txt}
	catch {exec ps.exe | grep.exe HomeFinder >> pid.txt}
	catch {exec ps.exe | grep.exe nameserv >> pid.txt}
	catch {exec ps.exe | grep.exe OTS >> pid.txt}
	catch {exec ps.exe | grep.exe RES >> pid.txt}
	catch {exec ps.exe | grep.exe DynDeploymentMgr >> pid.txt}
	
	#set ff [open "txt.txt" w]
	#fconfigure $ff -buffering line

	set fd [open "pid.txt" r]
	fconfigure $fd -buffering line
	
	while {![eof $fd]} {
		set line [gets $fd]
		regsub " +" $line " " line
		#puts  $ff $line
		set line [split $line]
		set pid [lindex $line 1]
		catch {exec kill.exe $pid}
	}

	close $fd
	#close $ff

	catch {exec rm.exe -rf pid.txt } errcode
	catch {exec rm.exe -rf global_id001.dat } errcode
	catch {exec rm.exe -rf output.txt } errcode
	catch {exec rm.exe -rf *-*-*-*-* } errcode
	catch {exec rm.exe -rf Repository/* } errcode
	catch {exec rm.exe -rf temp/*-*-*-*-* } errcode
	catch {exec rm.exe -rf temp/assembly } errcode
	catch {exec rm.exe -rf nodeid } errcode
	catch {exec rm.exe -rf state } errcode
	catch {exec rm.exe -rf *.ref } errcode
	catch {exec cp.exe ApplicationServer.xml.xml ApplicationServer.xml } errcode
}

proc aboutBox {} {
    tk_messageBox -icon info -type ok -title "About CCM Demo" -message \
"CCM Demo Controler, CCM team."
}

