#define	SHM_KEY_TRANS		100
#define	SHM_KEY_RES		101
#define	SHM_SIZE	1024
#define	SHM_ADDR	0

// DB type defination
#define Sybase	1
#define	Oracle	2
#define	Postgre	3 //renyi 11 19 : changed
#define Informix 4
//renyi 9 4:added
#define notDB    5

// the length of DBCHAR for data type: CHARBIND, NTBSTRINGBIND and STRINGBIND
// the length of char data type of C++
#define ARRAYLEN 	20	// the length of any array in SY_Resource.
#define STRINGLEN	200
			
#define _DEBUG_          // the output of screen
//#define _syb_PTHREAD_    // timeout thread to detect the deadlock of sybase
//#define _RECOVERY_       // write "./recovery_file" when coordinator commit the transaction
//#define _DEBUG_RECOVERY_ // "getchar" to debug the recovery in ots


