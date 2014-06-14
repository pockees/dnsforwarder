#include <string.h>
#include "common.h"
#include "querydnsinterface.h"
#include "querydnsbase.h"
#include "querydnslistentcp.h"
#include "querydnslistenudp.h"
#include "readconfig.h"
#include "hosts.h"
#include "excludedlist.h"
#include "gfwlist.h"
#include "utils.h"
#include "domainstatistic.h"
#include "debug.h"

static ConfigFileInfo	ConfigInfo;

int QueryDNSInterfaceInit(char *ConfigFile)
{
	VType	TmpTypeDescriptor;
	char	TmpStr[1024];

	GetFileDirectory(TmpStr);
	strcat(TmpStr, PATH_SLASH_STR);

	SetProgramEnvironment("PROGRAMDIRECTORY", TmpStr);

	ConfigInitInfo(&ConfigInfo);

    TmpTypeDescriptor.boolean = FALSE;
    ConfigAddOption(&ConfigInfo, "LogOn", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.INT32 = 102400;
    ConfigAddOption(&ConfigInfo, "LogFileThresholdLength", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

	GetFileDirectory(TmpStr);
	strcat(TmpStr, PATH_SLASH_STR);
    TmpTypeDescriptor.str = TmpStr;
    ConfigAddOption(&ConfigInfo, "LogFileFolder", STRATEGY_REPLACE, TYPE_PATH, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.str = "127.0.0.1";
    ConfigAddOption(&ConfigInfo, "LocalInterface", STRATEGY_REPLACE, TYPE_STRING, TmpTypeDescriptor, "Local working interface");

    TmpTypeDescriptor.INT32 = 53;
    ConfigAddOption(&ConfigInfo, "LocalPort", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, "Local working port");

    TmpTypeDescriptor.boolean = FALSE;
    ConfigAddOption(&ConfigInfo, "OpenLocalTCP", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, "Local TCP is opened");


    TmpTypeDescriptor.str = "TCP";
    ConfigAddOption(&ConfigInfo, "PrimaryServer", STRATEGY_REPLACE, TYPE_STRING, TmpTypeDescriptor, "Primary server");

    TmpTypeDescriptor.INT32 = 5;
    ConfigAddOption(&ConfigInfo, "UDPThreads", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.str = "8.8.8.8";
    ConfigAddOption(&ConfigInfo, "TCPServer", STRATEGY_APPEND_DISCARD_DEFAULT, TYPE_STRING, TmpTypeDescriptor, "TCP Server");

    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "UDPServer", STRATEGY_APPEND_DISCARD_DEFAULT, TYPE_STRING, TmpTypeDescriptor, "UDP Server");

    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "ExcludedDomain", STRATEGY_APPEND, TYPE_STRING, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "ExcludedList", STRATEGY_APPEND, TYPE_PATH, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.boolean = FALSE;
    ConfigAddOption(&ConfigInfo, "AllowFallBack", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.boolean = FALSE;
    ConfigAddOption(&ConfigInfo, "ParallelQuery", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, "UDP Parallel Query");

    TmpTypeDescriptor.boolean = FALSE;
    ConfigAddOption(&ConfigInfo, "UDPAntiPollution", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, "UDP Anti-pollution");

    TmpTypeDescriptor.boolean = FALSE;
    ConfigAddOption(&ConfigInfo, "UDPAppendEDNSOpt", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "UDPBlock_IP", STRATEGY_APPEND, TYPE_STRING, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.INT32 = 700;
    ConfigAddOption(&ConfigInfo, "TimeToServer", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "DedicatedServer", STRATEGY_APPEND, TYPE_STRING, TmpTypeDescriptor, NULL);


    TmpTypeDescriptor.boolean = FALSE;
    ConfigAddOption(&ConfigInfo, "DomainStatistic", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.INT32 = 60;
    ConfigAddOption(&ConfigInfo, "StatisticUpdateInterval", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

	ConfigAddAlias(&ConfigInfo, "StatisticFlushInterval", "StatisticUpdateInterval");


    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "Hosts", STRATEGY_APPEND, TYPE_STRING, TmpTypeDescriptor, "Hosts File");

    TmpTypeDescriptor.INT32 = 18000;
    ConfigAddOption(&ConfigInfo, "HostsUpdateInterval", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

	ConfigAddAlias(&ConfigInfo, "HostsFlushTime", "HostsUpdateInterval");

    TmpTypeDescriptor.INT32 = 30;
    ConfigAddOption(&ConfigInfo, "HostsRetryInterval", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

    ConfigAddAlias(&ConfigInfo, "HostsFlushTimeOnFailed", "HostsRetryInterval");

	GetFileDirectory(TmpStr);
	strcat(TmpStr, PATH_SLASH_STR);
	strcat(TmpStr, "hosts.txt");
    TmpTypeDescriptor.str = TmpStr;
    ConfigAddOption(&ConfigInfo, "HostsDownloadPath", STRATEGY_REPLACE, TYPE_PATH, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "HostsScript", STRATEGY_REPLACE, TYPE_PATH, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "AppendHosts", STRATEGY_APPEND, TYPE_STRING, TmpTypeDescriptor, NULL);

	ConfigAddAlias(&ConfigInfo, "address", "AppendHosts");


	TmpTypeDescriptor.boolean = TRUE;
    ConfigAddOption(&ConfigInfo, "UseCache", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, "Use cache");

    TmpTypeDescriptor.INT32 = 1048576;
    ConfigAddOption(&ConfigInfo, "CacheSize", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.boolean = TRUE;
    ConfigAddOption(&ConfigInfo, "MemoryCache", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, "Memory Cache");

    ConfigAddAlias(&ConfigInfo, "MemeryCache", "MemoryCache");

	GetFileDirectory(TmpStr);
	strcat(TmpStr, PATH_SLASH_STR);
	strcat(TmpStr, "cache");
    TmpTypeDescriptor.str = TmpStr;
    ConfigAddOption(&ConfigInfo, "CacheFile", STRATEGY_REPLACE, TYPE_PATH, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.boolean = FALSE;
    ConfigAddOption(&ConfigInfo, "IgnoreTTL", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, "Ignore TTL");

    TmpTypeDescriptor.INT32 = -1;
    ConfigAddOption(&ConfigInfo, "OverrideTTL", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

    ConfigAddAlias(&ConfigInfo, "ForceTTL", "OverrideTTL");

    TmpTypeDescriptor.INT32 = 1;
    ConfigAddOption(&ConfigInfo, "MultipleTTL", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.boolean = FALSE;
    ConfigAddOption(&ConfigInfo, "ReloadCache", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.boolean = FALSE;
    ConfigAddOption(&ConfigInfo, "OverwriteCache", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, NULL);


    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "DisabledType", STRATEGY_APPEND, TYPE_STRING, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "DisabledDomain", STRATEGY_APPEND, TYPE_STRING, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "DisabledList", STRATEGY_APPEND, TYPE_PATH, TmpTypeDescriptor, NULL);


    TmpTypeDescriptor.str = NULL;
    ConfigAddOption(&ConfigInfo, "GfwList", STRATEGY_REPLACE, TYPE_STRING, TmpTypeDescriptor, "GFW List");

    TmpTypeDescriptor.boolean = TRUE;
    ConfigAddOption(&ConfigInfo, "GfwListBase64Decode", STRATEGY_DEFAULT, TYPE_BOOLEAN, TmpTypeDescriptor, NULL);

    TmpTypeDescriptor.INT32 = 21600;
    ConfigAddOption(&ConfigInfo, "GfwListUpdateInterval", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

    ConfigAddAlias(&ConfigInfo, "GfwListFlushTime", "GfwListUpdateInterval");

    TmpTypeDescriptor.INT32 = 30;
    ConfigAddOption(&ConfigInfo, "GfwListRetryInterval", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

    ConfigAddAlias(&ConfigInfo, "GfwListFlushTimeOnFailed", "GfwListRetryInterval");

	GetFileDirectory(TmpStr);
	strcat(TmpStr, PATH_SLASH_STR);
	strcat(TmpStr, "gfwlist.txt");
    TmpTypeDescriptor.str = TmpStr;
    ConfigAddOption(&ConfigInfo, "GfwListDownloadPath", STRATEGY_REPLACE, TYPE_PATH, TmpTypeDescriptor, NULL);


    TmpTypeDescriptor.INT32 = 0;
    ConfigAddOption(&ConfigInfo, "RefusingResponseCode", STRATEGY_DEFAULT, TYPE_INT32, TmpTypeDescriptor, NULL);

	if( ConfigOpenFile(&ConfigInfo, ConfigFile) == 0 )
	{
		ConfigRead(&ConfigInfo);
		ConfigCloseFile(&ConfigInfo);
		return 0;
	} else {
		ERRORMSG("WARNING: Cannot load configuration file : %s, use default options. Use `-f' to specify another configure file.\n", ConfigFile);
		return 0;
	}
}

int QueryDNSInterfaceStart(void)
{
	int state = 0;

	const char *LocalAddr;

	int IsZeroZeroZeroZero;

	Debug_Init(&ConfigInfo);

	if( ShowMassages == TRUE )
	{
		ConfigDisplay(&ConfigInfo);
		putchar('\n');
	}

	InitAddress(&ConfigInfo);
	SetPrimaryProtocol(&ConfigInfo);

	srand(time(NULL));

	SetUDPAntiPollution(ConfigGetBoolean(&ConfigInfo, "UDPAntiPollution"));

	SetUDPAppendEDNSOpt(ConfigGetBoolean(&ConfigInfo, "UDPAppendEDNSOpt"));

	InitBlockedIP(ConfigGetStringList(&ConfigInfo, "UDPBlock_IP"));

	if( ConfigGetBoolean(&ConfigInfo, "DomainStatistic") == TRUE )
	{
		DomainStatistic_Init(ConfigGetInt32(&ConfigInfo, "StatisticUpdateInterval"));
	}

	ExcludedList_Init(&ConfigInfo);
	GfwList_Init(&ConfigInfo, FALSE);

	SetServerTimeOut(ConfigGetInt32(&ConfigInfo, "TimeToServer"));
	SetFallBack(ConfigGetBoolean(&ConfigInfo, "AllowFallBack"));

	DNSCache_Init(&ConfigInfo);

	if( ConfigGetInt32(&ConfigInfo, "UDPThreads") > 0)
	{
		if( QueryDNSListenUDPInit(&ConfigInfo) == 0 )
		{
			++state;
			QueryDNSListenUDPStart(&ConfigInfo);
		}
	}

	if( ConfigGetBoolean(&ConfigInfo, "OpenLocalTCP") == TRUE )
	{
		if( QueryDNSListenTCPInit(&ConfigInfo) == 0 )
		{
			++state;
			QueryDNSListenTCPStart(&ConfigInfo);
		}
	}

	if(state == 0)
	{
		return 1;
	}

	DynamicHosts_Init(&ConfigInfo);

	GfwList_PeriodicWork(&ConfigInfo);

	LocalAddr = ConfigGetRawString(&ConfigInfo, "LocalInterface");
	IsZeroZeroZeroZero = !strncmp(LocalAddr, "0.0.0.0", 7);
	INFO("Now you can set DNS%s%s.\n", IsZeroZeroZeroZero ? "" : " to ", IsZeroZeroZeroZero ? "" : LocalAddr);

	return 0;
}

void QueryDNSInterfaceWait(void)
{
#ifdef WIN32
	ThreadHandle CurrentThread = GetCurrentThread();
#endif /* WIN32 */

	if( ConfigGetBoolean(&ConfigInfo, "DomainStatistic") == TRUE )
	{
		DomainStatistic_Hold();
	} else {
		while(TRUE)
		{
#ifdef WIN32
			SuspendThread(CurrentThread);
#else /* WIN32 */
			pause();
#endif /* WIN32 */
		}
	}

#ifdef WIN32
	CloseHandle(CurrentThread);
#endif /* WIN32 */
}
