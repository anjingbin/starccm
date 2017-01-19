<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="Delegation">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="Delegation.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="Delegation.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="Delegation.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="Delegation.idl3"/>
	</localinterfacefile>
	<implementation id="8f42239b-1e67-11d7-b81f-dc8107a7889d">
		<code type="dll">
			<fileinarchive name="StdDelegation.dll"/>
			<entrypoint>createBankDelegationCompositionCCM_DelegationHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="Delegation.dll"/>
			<servantentrypoint>createBankDelegationHome</servantentrypoint>
			<contextentrypoint>createBankDelegationContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="Delegation.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
