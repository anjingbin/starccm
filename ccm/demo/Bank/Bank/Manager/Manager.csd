<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="Manager">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="Manager.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="Manager.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="Manager.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="Manager.idl3"/>
	</localinterfacefile>
	<implementation id="2aa2dbc5-2880-11d7-9c64-f6a003291bf5">
		<code type="dll">
			<fileinarchive name="StdManager.dll"/>
			<entrypoint>createBankManagerCompositionCCM_ManagerHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="Manager.dll"/>
			<servantentrypoint>createBankManagerHome</servantentrypoint>
			<contextentrypoint>createBankManagerContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="Manager.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
