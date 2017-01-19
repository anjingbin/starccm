<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="InterestComputer">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="InterestComputer.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="InterestComputer.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="InterestComputer.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="InterestComputer.idl3"/>
	</localinterfacefile>
	<implementation id="744697e3-287f-11d7-9c64-f6a003291bf5">
		<code type="dll">
			<fileinarchive name="StdInterestComputer.dll"/>
			<entrypoint>createBankInterestComputerCompositionCCM_InterestComputerHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="InterestComputer.dll"/>
			<servantentrypoint>createBankInterestComputerHome</servantentrypoint>
			<contextentrypoint>createBankInterestComputerContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="InterestComputer.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
