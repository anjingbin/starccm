<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="ATM">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="ATM.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="ATM.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="ATM.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="ATM.idl3"/>
	</localinterfacefile>
	<implementation id="d31e39d4-287f-11d7-9c64-f6a003291bf5">
		<code type="dll">
			<fileinarchive name="StdATM.dll"/>
			<entrypoint>createBankATMCompositionCCM_ATMHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="ATM.dll"/>
			<servantentrypoint>createBankATMHome</servantentrypoint>
			<contextentrypoint>createBankATMContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="ATM.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
