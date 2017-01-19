<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="DelAccountComp">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="DelAccountComp.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="DelAccountComp.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="DelAccountComp.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="DelAccountComp.idl3"/>
	</localinterfacefile>
	<implementation id="83f9e630-dace-40a7-854f-f6089f3c7364">
		<code type="dll">
			<fileinarchive name="StdDelAccountComp.dll"/>
			<entrypoint>createBankDelAccountCompCompositionCCM_DelAccountCompHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="DelAccountComp.dll"/>
			<servantentrypoint>createBankDelAccountCompHome</servantentrypoint>
			<contextentrypoint>createBankDelAccountCompContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="DelAccountComp.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
