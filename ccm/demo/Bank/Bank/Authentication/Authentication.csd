<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="Authentication">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="Authentication.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="Authentication.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="Authentication.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="Authentication.idl3"/>
	</localinterfacefile>
	<implementation id="1408d24a-2886-11d7-8609-d971ee33e54a">
		<code type="dll">
			<fileinarchive name="StdAuthentication.dll"/>
			<entrypoint>createBankAuthenticationCompositionCCM_AuthenticationHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="Authentication.dll"/>
			<servantentrypoint>createBankAuthenticationHome</servantentrypoint>
			<contextentrypoint>createBankAuthenticationContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="Authentication.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
