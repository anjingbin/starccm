<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="cutlery">
	<ccdskelfile>
		<fileinarchive name="Cutlery.ccd"/>
	</ccdskelfile>
	<idl3file>
		<fileinarchive name="Dinner.idl3"/>
	</idl3file>
	<idl2file>
		<fileinarchive name="Dinner.idl"/>
	</idl2file>
	<cidlfile>
		<fileinarchive name="Dinner.cidl"/>
	</cidlfile>
	<localinterfacefile>
		<fileinarchive name="CCM_Dinner.idl"/>
	</localinterfacefile>
	<implementation id="bb38d965-1e54-11d7-9b85-8a6194305b45">
		<code type="dll">
			<fileinarchive name="StdDinner.dll"/>
			<entrypoint>createDinnerCutleryImplCCM_CutleryHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="Dinner.dll"/>
			<servantentrypoint>createDinnerCutleryHome</servantentrypoint>
			<contextentrypoint>createDinnerCutleryContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="cutlery.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
