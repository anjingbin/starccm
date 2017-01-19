<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE softpkg PUBLIC "" "softpkg.dtd">
<softpkg version="1.0" name="philosopher">
	<ccdskelfile>
		<fileinarchive name="Ccd.skel"/>
	</ccdskelfile>
	<componentcpfskelfile>
		<fileinarchive name="ComponentCpf.skel"/>
	</componentcpfskelfile>
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
	<implementation id="8f42239b-1e67-11d7-b81f-dc8107a7889d">
		<code type="dll">
			<fileinarchive name="StdDinner.dll"/>
			<entrypoint>createDinnerPhilosopherImplCCM_PhilosopherHome_impl</entrypoint>
		</code>
		<servantcode type="dll">
			<fileinarchive name="Dinner.dll"/>
			<servantentrypoint>createDinnerPhilosopherHome</servantentrypoint>
			<contextentrypoint>createDinnerPhilosopherContext</contextentrypoint>
		</servantcode>
		<descriptor type="CORBA Component">
			<fileinarchive name="philosopher.ccd"/>
		</descriptor>
	</implementation>
</softpkg>
