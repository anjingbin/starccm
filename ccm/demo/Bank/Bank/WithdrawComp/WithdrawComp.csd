<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="WithdrawComp">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="WithdrawComp.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="WithdrawComp.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="WithdrawComp.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="WithdrawComp.idl3"/>
	</localinterfacefile>
	<implementation id="f8f3fc5d-4725-4c5b-b814-157c0f71c589">
		<code type="dll">
			<fileinarchive name="StdWithdrawComp.dll"/>
			<entrypoint>createBankWithdrawCompCompositionCCM_WithdrawCompHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="WithdrawComp.dll"/>
			<servantentrypoint>createBankWithdrawCompHome</servantentrypoint>
			<contextentrypoint>createBankWithdrawCompContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="WithdrawComp.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
