<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="DepositComp">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="DepositComp.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="DepositComp.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="DepositComp.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="DepositComp.idl3"/>
	</localinterfacefile>
	<implementation id="d35275cc-286a-11d7-8c80-8fca68dc242e">
		<code type="dll">
			<fileinarchive name="StdDepositComp.dll"/>
			<entrypoint>createBankDepositCompCompositionCCM_DepositCompHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="DepositComp.dll"/>
			<servantentrypoint>createBankDepositCompHome</servantentrypoint>
			<contextentrypoint>createBankDepositCompContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="DepositComp.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
