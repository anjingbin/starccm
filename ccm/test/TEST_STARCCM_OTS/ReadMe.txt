// **********************************************************************//
//                        STARCCM��OTS���ɲ���˵��                       //
// **********************************************************************//

�ڱ��ļ�����������CCMӦ�÷������й��ڼ���OTS���ֵĲ��ԡ�
��STARCCM_CMTĿ¼�������е�������������ʽ�Ĳ��ԡ�
��STARCCM_SMTĿ¼�������е��Թ�������ʽ�Ĳ��ԡ�


��1��STARCCM1.0 CMT��ʽ���ԣ�


��STARCCM_CMT�ļ����а��������ļ��У�
	src ------- ������Դ�ļ��У����з�����CMT��ʽ�µĹ���ʵ���ļ���Client��ʵ���ļ���
	idl ------- ������IDL3 PSDL CIDL�ļ���
	bin ------- src���ļ�ͨ����������ɵ��ļ���

��src �ļ����е��ļ�˵����
	CCM_HelloWorld_impl.cpp ----- ����ʵ��Դ�ļ���
	CCM_HelloWorld_impl.h ------- ����ʵ��ͷ�ļ���
	REQUIRES_NEW_Client.cpp ----- REQUIRES_NEW�����¹����Ŀͻ���Դ�ļ���
	MANDATORY_Client.cpp -------- MANDATORY�����¹����Ŀͻ���Դ�ļ���
	NEVER_Client.cpp ------------ NEVER�����¹����Ŀͻ���Դ�ļ���
	REQUIRED_Client.cpp --------- REQUIRED�����¹����Ŀͻ���Դ�ļ���
	NOT_SUPPORTED_Client.cpp ---- NOT_SUPPORTED�����¹����Ŀͻ���Դ�ļ���
	SUPPORTS_Client.cpp --------- SUPPORTS�����¹����Ŀͻ���Դ�ļ���


�ڽ���CMT��ʽ�Ĳ���ʱ����CCM�İ�װ·������Ҫ�޸ĵ��ļ����£�
	1. HomeFinder_IP.txt��Ӧ�÷��������е�IP��ַ���ڲ�ͬ�����л�������Ҫ�޸ĸ��ļ��е�IP��ַ��
	�����޸�ΪӦ�÷�������IP��
	2. starccm.ini�ļ��а����������֣���main_host_ip ����ΪӦ�÷�����������IP��ַ��������host_ip��ַ��
	�ڱ����������н������õ�IP��ַ��main_host_ip ��ͬ��
	3. ��Start_StarCCM_OTS_test.pl�ļ������ı��༭���򿪣��ڵ�101���У���system("cmd /C Deploy -package 	REQUIRES_NEW_Package.zip &")��������Ҫ���Ե������������ڴ���REQUIRES_NEW_Package.zip�����Ҫ������������������Ϊ
	���ļ�����

�����е�XXX_Package�ļ����У��Ƕ���������ԵĹ�����������Щ�ļ����а���һ��*.cad�ļ��������е�
IP��ַ�滻ΪӦ�÷��������е�IP��ַ�����ñ�׼��zip����hello.cad��person�ļ���ѹ�������



���в��������Ĳ��裺
	1.��HomeFinder_IP.txt��starccm.ini�ļ��е����ݰ��������ļ��Ĺ��ܽ����޸ģ�
	2.�����е�XXX_package�ļ����У��Ƕ���������ԵĹ�����������Щ�ļ����а���һ��*.cad�ļ��������е�
	IP��ַ�滻ΪӦ�÷��������е�IP��ַ�����ñ�׼��zip����hello.cad��person�ļ���ѹ�������	
	3.��Start_StarCCM_OTS_test.pl�����޸ģ��޸ķ����������Ѿ�����,�����޸�Ϊ������ԵĹ��������ļ�����
	4.����Start_StarCCM_OTS_test.pl������Ӧ�÷��������������ڴ��ļ������õĹ�������
	5.������Start_StarCCM_OTS_test.pl�в���Ĺ��������Ӧ�Ŀͻ��˿�ִ���ļ������磺��Start_StarCCM_OTS_test.pl�в������		SUPPORTS_Package.zip��������SUPPORTS_Client.exe����
	6.������ɺ���Ҫ������stop_server.pl����Ӧ�÷������رգ��ٽ�������Ĳ��ԣ���Ҫ�ӵ�1�����¿�ʼ��




��2��STARCCM1.0 SMT��ʽ���ԣ�


��STARCCM_SMT�ļ����а��������ļ��У�
	src ------- ������Դ�ļ��У����з�����SMT��ʽ�µĹ���ʵ���ļ���Client��ʵ���ļ���
	idl ------- ������IDL3 PSDL CIDL�ļ���
	bin ------- src���ļ�ͨ����������ɵ��ļ���

��src �ļ����е��ļ�˵����
	CCM_HelloWorld_impl.cpp ----- ����ʵ��Դ�ļ���
	CCM_HelloWorld_impl.h ------- ����ʵ��ͷ�ļ���
	SMT_Client.cpp -------------- self-managed�����¹����Ŀͻ���Դ�ļ���



���в��������Ĳ��裺
	1.��HomeFinder_IP.txt��starccm.ini�ļ��е����ݰ��������ļ��Ĺ��ܽ����޸ģ�
	2.��SMT_Package.zip�ļ��У��Ƕ���������ԵĹ�����������Щ�ļ����а���һ��*.cad�ļ��������е�
	IP��ַ�滻ΪӦ�÷��������е�IP��ַ�����ñ�׼��zip����hello.cad��person�ļ���ѹ�������	
	3.��Start_StarCCM_OTS_test.pl�����޸ģ��޸ķ����������Ѿ�����,�����޸�Ϊ������ԵĹ��������ļ�����
	4.����Start_StarCCM_OTS_test.pl������Ӧ�÷��������������ڴ��ļ������õĹ�������
	5.������Start_StarCCM_OTS_test.pl�в���Ĺ��������Ӧ�Ŀͻ��˿�ִ���ļ������磺��Start_StarCCM_OTS_test.pl�в������		SMT_Package.zip��������SMT_Client.exe����
	6.������ɺ���Ҫ������stop_server.pl����Ӧ�÷������رգ��ٽ�������Ĳ��ԣ���Ҫ�ӵ�1�����¿�ʼ��
