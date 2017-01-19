<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="OpenAccountComp">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="OpenAccountComp.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="OpenAccountComp.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="OpenAccountComp.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="OpenAccountComp.idl3"/>
	</localinterfacefile>
	<implementation id="dd16da20-577d-4275-96fa-705384422a1f">
		<code type="dll">
			<fileinarchive name="StdOpenAccountComp.dll"/>
			<entrypoint>createBankOpenAccountCompCompositionCCM_OpenAccountCompHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="OpenAccountComp.dll"/>
			<servantentrypoint>createBankOpenAccountCompHome</servantentrypoint>
			<contextentrypoint>createBankOpenAccountCompContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="OpenAccountComp.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
