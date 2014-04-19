#ifndef _QUERY_DNS_BASE_H_
#define _QUERY_DNS_BASE_H_

#include "debug.h"

typedef enum _DnsQuaryProtocol{
	DNS_QUARY_PROTOCOL_UDP = 0,
	DNS_QUARY_PROTOCOL_TCP = 1
} DNSQuaryProtocol;

#include <time.h>
#include "common.h"
#include "dnscache.h"
#include "readconfig.h"
#include "extendablebuffer.h"
#include "request_response.h"

typedef struct _QueryContext ThreadContext;

struct _QueryContext{
	ThreadContext	*Head;
	ThreadContext	*Previous;

	SOCKET	TCPSocket;
	struct	sockaddr	*LastServer;

	SOCKET	UDPSocket;
	sa_family_t	LastFamily;

	SOCKET	*PrimarySocket;
	SOCKET	*SecondarySocket;

	DNSQuaryProtocol	PrimaryProtocolToServer;

	char				*RequestEntity;
	int					RequestLength;
	const char			*RequestingDomain;
	DNSRecordType		RequestingType;
	int					RequestingDomainHashValue;

	const char			*ClientIP;

	BOOL	Compress;

	time_t	CurrentTime;


	ExtendableBuffer	*ResponseBuffer;

	/* Do not refer this, let `ResponseBuffer' point to this and use `ResponseBuffer' instead */
	ExtendableBuffer	ResponseBuffer_Entity;

};

void SetFallBack(BOOL FallBack);

void ShowRefusingMassage(ThreadContext *Context, const char *Massage);

void ShowErrorMassage(ThreadContext *Context, char ProtocolCharacter);

void ShowNormalMassage(ThreadContext *Context, int32_t Offset, char ProtocolCharacter);

void ShowBlockedMessage(const char *RequestingDomain, const char *Package, const char *Message);

int DNSFetchFromCache(__in ThreadContext *Context);

int InitAddress(ConfigFileInfo *ConfigInfo);

void SetPrimaryProtocol(ConfigFileInfo *ConfigInfo);

int FetchFromHostsAndCache(ThreadContext *Context);

#define QUERY_RESULT_DISABLE	(-1)
#define QUERY_RESULT_ERROR		(-2)

int QueryBase(ThreadContext *Context);

void InitContext(ThreadContext *Context, char *RequestEntity);

int	GetAnswersByName(ThreadContext *Context, const char *Name, DNSRecordType Type, const char *Agent);

int GetHostsByRaw(const char *RawPackage, StringList *out);

int GetHostsByName(const char *Name, const char *Agent, StringList *out);

int GetMaximumMessageSize(SOCKET sock);

#endif /* _QUERY_DNS_BASE_H_ */
