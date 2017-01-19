<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="QueryComp">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="QueryComp.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="QueryComp.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="QueryComp.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="QueryComp.idl3"/>
	</localinterfacefile>
	<implementation id="fcda6382-287e-11d7-9c64-f6a003291bf5">
		<code type="dll">
			<fileinarchive name="StdQueryComp.dll"/>
			<entrypoint>createBankQueryCompCompositionCCM_QueryCompHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="QueryComp.dll"/>
			<servantentrypoint>createBankQueryCompHome</servantentrypoint>
			<contextentrypoint>createBankQueryCompContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="QueryComp.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
