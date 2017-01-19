<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="Account">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="Account.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="Account.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="Account.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="Account.idl3"/>
	</localinterfacefile>
	<implementation id="93e1e9a6-2880-11d7-9c64-f6a003291bf5">
		<code type="dll">
			<fileinarchive name="StdAccount.dll"/>
			<entrypoint>createBankAccountCompositionCCM_AccountHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="Account.dll"/>
			<servantentrypoint>createBankAccountHome</servantentrypoint>
			<contextentrypoint>createBankAccountContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="Account.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
