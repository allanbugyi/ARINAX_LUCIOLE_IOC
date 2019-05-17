/***************************************************************************/
/*                                                                         */
/*                               Lucioldll.h                               */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Definitions and functions declarations  for Luciole communication dll   */
/*                                                                         */
/***************************************************************************/
/* Creation     : 20.08.2015  J. NASRI         Version 1.00                */
/* Update       : 09.09.2015  B. LAVAULT       Version 1.1                 */
/***************************************************************************/

#ifndef __LUCIOLDLL_H__
#define __LUCIOLDLL_H__

typedef char BYTE;
typedef short WORD;
typedef int DWORD;
typedef bool BOOL;
typedef long h_TinyDiaFrame;

#ifdef __cplusplus
extern "C" {
#endif
#ifndef _WIN32
        #define __stdcall
#endif


#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
        #ifdef LUCIOLDLL_EXPORTS
            #ifdef __GNUC__
                #define LUCIOLDLL_API __attribute__ ((dllexport))
            #else
                #define LUCIOLDLL_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
            #endif
        #else
            #ifdef __GNUC__
                #define LUCIOLDLL_API __attribute__ ((dllimport))
            #else
                #define LUCIOLDLL_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
            #endif
        #endif
#else
        #if __GNUC__ >= 4
            #define LUCIOLDLL_API __attribute__ ((visibility ("default")))
        #else
            #define LUCIOLDLL_API 
        #endif
#endif

	/*-------------------------------------------------------------------------*/
	/* Definitions                                                             */
	/*-------------------------------------------------------------------------*/

	typedef enum 							
	{
		/* Flash Variables */
		cls_IP_CONFIG = 1,					/* VarId-01 : configuration Ip: r/w          */
		cls_ETH_CONFIG = 2,					/* VarId-02 : configuration Ethernet: r/w    */
		cls_TCP_PORT_NUM=3,					/* VarId-03 : Tcp Port Number: r/w           */
		cls_CH1_OFFSET_MAN=4,				/* VarId-04 : Offest_CH1_Manual: r/w         */
		cls_CH2_OFFSET_MAN=5,				/* VarId-05 : Offest_CH2_Manual: r/w         */
		cls_CH1_OFFSET_REM=6,				/* VarId-06 : Offest_CH1_Remote: r/w         */
		cls_CH2_OFFSET_REM=7,				/* VarId-07 : Offest_CH2_Remote: r/w         */
		cls_CH1_RATIOMAX_MAN=8,				/* VarId-08 : RatioMax_CH1_Manual: r/w       */
		cls_CH2_RATIOMAX_MAN=9,				/* VarId-09 : RatioMax_CH2_Manual: r/w       */
		cls_CH1_RATIOMAX_REM=10,			/* VarId-10 : RatioMax_CH1_Remote: r/w       */
		cls_CH2_RATIOMAX_REM=11,			/* VarId-11 : RatioMax_CH2_Remote: r/w       */

		/*  Volatile Variables */
		cls_CH1_DIG_COMMAND=101,			/* VarId-101 : digital Command ch1: r/w      */
		cls_CH2_DIG_COMMAND=102,			/* VarId-102 : digital Command ch2: r/w      */
		cls_LED1_TEMP=103,					/* VarId-103 : Temperature LED1: r           */
		cls_LED21_TEMP=104,					/* VarId-104 : Temperature LED2_1: r         */
		cls_LED22_TEMP=105,					/* VarId-105 : Temperature LED2_2: r         */
		cls_OVERTEMP_ERR=106,				/* VarId-106 : NO:1;CH1=2;CH2=3;CH1&CH2=4: r */
		cls_CH1_TRIG_MODE=107,				/* VarId-107 : CH1 trigger mode=1/0: r/w     */
		cls_CH2_TRIG_MODE=108,				/* VarId-108 : CH2 trigger mode=1/0: r/w     */
		cls_CH1_CTRL_MODE=109,			    /* VarId-109 : CH1 CTRL net=4;Rem=2;Man=1: r */
		cls_CH2_CTRL_MODE=110,			    /* VarId-110 : CH2 CTRL net=4;Rem=2;Man=1: r */
		/* VarId-111->VarId-119: Reserved            */
		cls_CH2_SET_NETWORK_MODE=120		/* VarId-120 : SetNetworkMode :CH1=1;CH2=: w */

	} cls_VarID ;

	typedef enum							/* TCP PARAM ID */
	{
		cls_TCP_DHCP = 1,					/* command 91_1 VarId-01 : DHCP Mode  r/w                */
		cls_TCP_IP = 2,					    /* command 91_1 VarId-02 : IP Adress  r/w                */
		cls_TCP_SUBNETMASK=3,				/* command 91_1 VarId-03 : SubNetMask: r/w               */
		cls_CH1_GATEWAY=4,				    /* command 91_1 VarId-04 : Gateway: r/w                  */
		cls_TCP_SPEED = 5,                 /* select command 91_2              */
        cls_TCP_PORT = 6                   /* select command 91_3              */

	} cls_TCPParamID ;

	typedef enum							/* RW Access                                 */
	{
		cls_READ = 0,					    /* READ Access                               */
		cls_WRITE = 1					    /* WRITE Access                              */	

	} cls_RWAccess ;

	typedef enum							/* RW Access                                 */
	{
		SER = 1,					        /*SERIAL COM */
		TCP = 2,					        /*TCP COM    */

	} cls_COMTYPE;

	typedef enum							/* RW Access */
	{
		COM1 = 1,					        /*SERIAL COM xx */
		COM2 = 2,					    
		COM3 = 3,					    
		COM4 = 4,					    

	} cls_SerialCOM;

	typedef enum	/* R Access */
	{
		MANUAL = 1,	/* Control with the physical button */
		ANALOG = 2, /* Control with the analog lines */                  					    
		REMOTE = 4	/* Control with ethernet commands */     

	} cls_ControlMode;

	typedef enum	
	{
		SPEED_100_FULL = 0,	
		SPEED_AUTO_NEG = 1, 
		SPEED_100_HALF = 0,
		SPEED_10_FULL = 4,
		SPEED_10_HALF = 6

	} cls_SpeedMode;

	typedef enum	/* luciol DLL internal ERROR codes (not from Diaframe) */			
	{
		SUCCESS            = 0,
		ERROR_INVALID_CMD  = -20,   /* Unsupported command/variable number */
		ERROR_NO_RESP      = -1000,	/* No response */
		ERROR_COM_NOT_OPEN = -3000,	/* Not open */
		ERROR_COM_PROBLEM  = -2000,	/* Sending/reception problem */
		ERROR_INVALID_PARAM  = -4000  /* Function arguments are not valid. E.g: Channel is not 1 or 2 */

	} cls_Error;

	/*-------------------------------------------------------------------------*/
	/* Definitions FOR  DLL                                                    */
	/*-------------------------------------------------------------------------*/

	/*-------------------------------------------------------------------------*/
	/* High Level functions                                                    */                                                  
	/*-------------------------------------------------------------------------*/
	LUCIOLDLL_API int cls_SetSerialInterface(const char* strCOM, const char* strBaudRate);
	LUCIOLDLL_API int cls_SetEthernetInterface(char * strIpAddr);
	LUCIOLDLL_API int cls_SetLightValue(BYTE Chx , WORD LightVal);
	LUCIOLDLL_API int cls_GetLightValue(BYTE Chx);
	LUCIOLDLL_API void cls_GetComConfig(char **pstrconfig);
	LUCIOLDLL_API void cls_GetState(char **pstrError);
	LUCIOLDLL_API void cls_Close();
	LUCIOLDLL_API void cls_Init();
	LUCIOLDLL_API int cls_GetControlMode(BYTE Chx);
	LUCIOLDLL_API int cls_ReleaseNetworkMode(BYTE Chx);
	LUCIOLDLL_API int cls_GetTrigger(BYTE Chx);
	LUCIOLDLL_API int cls_SetTrigger(BYTE Chx, int Flag);
	LUCIOLDLL_API BOOL cls_IsConnected();

	/*-------------------------------------------------------------------------*/
	/* Admin function                                                          */
	/*-------------------------------------------------------------------------*/
	LUCIOLDLL_API long cls_ReadTCPParam(cls_TCPParamID TCPParamId);
	LUCIOLDLL_API int cls_WriteTCPParam(cls_SpeedMode Speed, int Port, BYTE DHCPMode, DWORD IpVal, DWORD SubNetMaskVal, DWORD GatewayVal);
	LUCIOLDLL_API int cls_ReadChVar(cls_VarID VarId) ;
	LUCIOLDLL_API int cls_WriteChVar(cls_VarID VarId , WORD VarVal);

	/*-------------------------------------------------------------------------*/
	/* Low Level functions                                                     */
	/*-------------------------------------------------------------------------*/

	LUCIOLDLL_API h_TinyDiaFrame cls_OpenCom(char * g_strPortDesc );
	LUCIOLDLL_API long cls_WriteTCPParamEx(h_TinyDiaFrame hDiaFrame, BYTE DHCPMode , DWORD IpVal,DWORD SubNetMaskVal,DWORD GatewayVal);
	LUCIOLDLL_API long cls_ReadTCPParamEx(h_TinyDiaFrame hDiaFrame, cls_TCPParamID TCPParamId);
	LUCIOLDLL_API int cls_ReadWriteChVarEx(h_TinyDiaFrame hDiaFrame,cls_RWAccess ReadWrite,cls_VarID VarId , WORD VarVal);
	LUCIOLDLL_API void cls_CloseCom(h_TinyDiaFrame hDiaFrame);

#ifdef __cplusplus
}
#endif


#endif  /* __LUCIOL_H__ */
