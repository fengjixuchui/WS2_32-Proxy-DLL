#include <WS2tcpip.h>
#include <Windef.h>
#include <windows.h>
#include <winsock2.h>

#include "Log.h"

HMODULE iWS232 = NULL;
FARPROC aWS232;

typedef int(CALLBACK * LPCONDITIONPROC)(LPWSABUF lpCallerId, LPWSABUF lpCallerData, LPQOS lpSQOS, LPQOS lpGQOS, LPWSABUF lpCalleeId, LPWSABUF lpCalleeData, GROUP FAR * g, DWORD dwCallbackData);

//0x0001 0x00011040 "accept" 
int(_stdcall *_accept) (SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen);
//0x0002 0x00004480 "bind" 
int(_stdcall *_bind) (SOCKET, const struct sockaddr *, int);
//0x0003 0x00003E2B "closesocket" 
int(_stdcall *_closesocket) (SOCKET);
//0x0004 0x00004A07 "connect" 
int(_stdcall *_connect) (SOCKET, const struct sockaddr *, int);
//0x0005 0x00010B68 "getpeername" 
int(_stdcall *_getpeername)(SOCKET s, struct sockaddr FAR *name, int FAR *namelen);
//0x0006 0x00003D10 "getsockname" 
int(_stdcall *_getsockname) (SOCKET, struct sockaddr *, int *);
//0x0007 0x000046EA "getsockopt" 
int(_stdcall *_getsockopt) (SOCKET, int, int, char *, int *);
//0x0008 0x00002EAD "htonl" 
u_long(_stdcall *_htonl) (u_long);
//0x0009 0x00002E53 "htons" 
u_short(_stdcall *_htons) (u_short);
//0x000A 0x00003F50 "ioctlsocket" 
int(_stdcall *_ioctlsocket) (SOCKET, long, u_long FAR *);
//0x000B 0x00002EE1 "inet_addr" 
unsigned long(_stdcall *_inet_addr) (const char FAR *);
//0x000C 0x000045C1 "inet_ntoa" 
char * (_stdcall *_inet_ntoa) (struct in_addr);
//0x000D 0x00008CD3 "listen" 
int(_stdcall *_listen)(SOCKET s, int backlog);
//0x000E 0x00002EAD "ntohl" 
u_long(_stdcall *_ntohl)(u_long netlong);
//0x000F 0x00002E53 "ntohs" 
u_short(_stdcall *_ntohs) (u_short);
//0x0010 0x0000676F "recv" 
int(_stdcall *_recv) (SOCKET, char FAR *, int, int);
//0x0011 0x00002FF7 "recvfrom" 
int(_stdcall *_recvfrom)(SOCKET s, char FAR *buf, int len, int flags, struct sockaddr FAR *from, int FAR *fromlen);
//0x0012 0x000030A8 "select" 
int(_stdcall *_select) (int, fd_set FAR *, fd_set FAR *, fd_set FAR *, const struct timeval FAR *);
//0x0013 0x00004C27 "send" 
int(_stdcall *_send) (SOCKET, const char *, int, int);
//0x0014 0x00002F51 "sendto" 
int(_stdcall *_sendto)(SOCKET s, const char FAR *buf, int len, int flags, const struct sockaddr FAR *to, int tolen);
//0x0015 0x00004521 "setsockopt" 
int(_stdcall *_setsockopt) (SOCKET, int, int, const char *, int);
//0x0016 0x00010BF6 "shutdown" 
int(_stdcall *_shutdown) (SOCKET s, int how);
//0x0017 0x00004211 "socket" 
SOCKET(_stdcall *_socket) (int, int, int);
//0x0018 0x00002899 "GetAddrInfoW" 
int(_stdcall *_GetAddrInfoW)(IN PCWSTR pNodeName, IN PCWSTR pServiceName, IN const ADDRINFOW *pHints, OUT PADDRINFOW *ppResult);
//0x0019 0x0000C504 "GetNameInfoW" 
int(_stdcall *_GetNameInfoW)(IN const SOCKADDR *pSockaddr, IN socklen_t SockaddrLength, OUT PWCHAR pNodeBuffer, IN DWORD NodeBufferSize, OUT PWCHAR pServiceBuffer, IN DWORD ServiceBufferSize, IN int Flags);
//0x001A 0x00011EE1 "WSApSetPostRoutine" 

//0x001B 0x00002B0B "FreeAddrInfoW" 
void(_stdcall *_FreeAddrInfoW)(IN PADDRINFOW pAddrInfo);
//0x001C 0x00011CBF "WPUCompleteOverlappedRequest" 
int(_stdcall *_WPUCompleteOverlappedRequest) (SOCKET s, LPWSAOVERLAPPED lpOverlapped, DWORD dwError, DWORD cbTransferred, LPINT lpErrno);
//0x001D 0x00010DC1 "WSAAccept" 
SOCKET(_stdcall *_WSAAccept)(SOCKET s, struct sockaddr FAR *addr, LPINT addrlen, LPCONDITIONPROC lpfnCondition, DWORD dwCallbackData);
//0x001E 0x00009462 "WSAAddressToStringA" 
int(_stdcall *_WSAAddressToStringA)(IN LPSOCKADDR lpsaAddress, IN DWORD dwAddressLength, IN LPWSAPROTOCOL_INFOA lpProtocolInfo, IN OUT LPSTR lpszAddressString, IN OUT LPDWORD lpdwAddressStringLength);
//0x001F 0x00006979 "WSAAddressToStringW" 
int(_stdcall *_WSAAddressToStringW)(IN LPSOCKADDR lpsaAddress, IN DWORD dwAddressLength, IN LPWSAPROTOCOL_INFOW lpProtocolInfo, IN OUT LPWSTR lpszAddressString, IN OUT LPDWORD lpdwAddressStringLength);
//0x0020 0x000065E8 "WSACloseEvent" 
BOOL(_stdcall * _WSACloseEvent)(WSAEVENT hEvent);
//0x0021 0x00010C81 "WSAConnect" 
int(_stdcall *_WSAConnect)(SOCKET s, const struct sockaddr FAR *name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS);
//0x0022 0x0000655D "WSACreateEvent" 
WSAEVENT(_stdcall * _WSACreateEvent)(void);
//0x0023 0x0000DB9A "WSADuplicateSocketA" 
int(_stdcall *_WSADuplicateSocketA)(SOCKET s, DWORD dwProcessId, LPWSAPROTOCOL_INFOA lpProtocolInfo);
//0x0024 0x0000DB0C "WSADuplicateSocketW" 
int(_stdcall *_WSADuplicateSocketW)(SOCKET s, DWORD dwProcessId, LPWSAPROTOCOL_INFOW lpProtocolInfo);
//0x0025 0x0000F9E1 "WSAEnumNameSpaceProvidersA" 
int(_stdcall *_WSAEnumNameSpaceProvidersA)(IN OUT LPDWORD lpdwBufferLength, IN LPWSANAMESPACE_INFOA lpnspBuffer);
//0x0026 0x0000FA41 "WSAEnumNameSpaceProvidersW" 
int(_stdcall *_WSAEnumNameSpaceProvidersW)(IN OUT LPDWORD lpdwBufferLength, IN LPWSANAMESPACE_INFOW lpnspBuffer);
//0x0027 0x0000657D "WSAEnumNetworkEvents" 
int(_stdcall *_WSAEnumNetworkEvents)(SOCKET s, WSAEVENT hEventObject, LPWSANETWORKEVENTS lpNetworkEvents);
//0x0028 0x0000DC5F "WSAEnumProtocolsA" 
int(_stdcall *_WSAEnumProtocolsA)(LPINT lpiProtocols, LPWSAPROTOCOL_INFOA lpProtocolBuffer, LPDWORD lpdwBufferLength);
//0x0029 0x0000888C "WSAEnumProtocolsW" 
int(_stdcall *_WSAEnumProtocolsW)(LPINT lpiProtocols, LPWSAPROTOCOL_INFOW lpProtocolBuffer, LPDWORD lpdwBufferLength);
//0x002A 0x000064D9 "WSAEventSelect" 
int(_stdcall *_WSAEventSelect)(SOCKET s, WSAEVENT hEventObject, long lNetworkEvents);
//0x002B 0x00010D1B "WSAGetOverlappedResult" 
BOOL(_stdcall * _WSAGetOverlappedResult)(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags);
//0x002C 0x0000F470 "WSAGetQOSByName" 
BOOL(_stdcall * _WSAGetQOSByName)(SOCKET s, LPWSABUF lpQOSName, LPQOS lpQOS);
//0x002D 0x00010440 "WSAGetServiceClassInfoA" 
int(_stdcall *_WSAGetServiceClassInfoA)(IN LPGUID lpProviderId, IN LPGUID lpServiceClassId, IN OUT LPDWORD lpdwBufSize, OUT LPWSASERVICECLASSINFOA lpServiceClassInfo);
//0x002E 0x00010159 "WSAGetServiceClassInfoW" 
int(_stdcall *_WSAGetServiceClassInfoW)(IN LPGUID lpProviderId, IN LPGUID lpServiceClassId, IN OUT LPDWORD lpdwBufSize, OUT LPWSASERVICECLASSINFOW lpServiceClassInfo);
//0x002F 0x0000FD9F "WSAGetServiceClassNameByClassIdA" 
int(_stdcall *_WSAGetServiceClassNameByClassIdA)(IN LPGUID lpServiceClassId, OUT LPSTR lpszServiceClassName, IN OUT LPDWORD lpdwBufferLength);
//0x0030 0x0000FF81 "WSAGetServiceClassNameByClassIdW" 
int(_stdcall *_WSAGetServiceClassNameByClassIdW)(IN LPGUID lpServiceClassId, OUT LPWSTR lpszServiceClassName, IN OUT LPDWORD lpdwBufferLength);
//0x0031 0x0000BCE1 "WSAHtonl" 
int(_stdcall *_WSAHtonl)(SOCKET s, u_long hostlong, u_long FAR *lpnetlong);
//0x0032 0x0000BDC9 "WSAHtons" 
int(_stdcall *_WSAHtons)(SOCKET s, u_short hostshort, u_short FAR *lpnetshort);
//0x0033 0x0000E491 "gethostbyaddr" 
struct hostent * (_stdcall *_gethostbyaddr)(const char FAR *addr, int len, int type);
//0x0034 0x00005355 "gethostbyname" 
struct hostent * (_stdcall *_gethostbyname) (const char FAR *);
//0x0035 0x0000E18E "getprotobyname" 
struct protoent * (_stdcall *_getprotobyname)(const char FAR *name);
//0x0036 0x0000E0CC "getprotobynumber" 
struct protoent * (_stdcall *_getprotobynumber)(int proto);
//0x0037 0x0000E703 "getservbyname" 
struct servent * (_stdcall *_getservbyname)(const char FAR *name, const char FAR *proto);
//0x0038 0x0000E5B5 "getservbyport" 
struct servent * (_stdcall *_getservbyport)(int port, const char FAR *proto);
//0x0039 0x00005449 "gethostname" 
int(_stdcall *_gethostname) (char FAR *, int);
//0x003A 0x000103C1 "WSAInstallServiceClassA" 
int(_stdcall *_WSAInstallServiceClassA)(IN LPWSASERVICECLASSINFOA lpServiceClassInfo);
//0x003B 0x0000FBC9 "WSAInstallServiceClassW" 
int(_stdcall *_WSAInstallServiceClassW)(IN LPWSASERVICECLASSINFOW lpServiceClassInfo);
//0x003C 0x00003EC0 "WSAIoctl" 
int(_stdcall *_WSAIoctl)(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer, DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
//0x003D 0x00010F70 "WSAJoinLeaf" 
SOCKET(_stdcall *_WSAJoinLeaf)(SOCKET s, const struct sockaddr FAR *name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS, DWORD dwFlags);
//0x003E 0x0000562A "WSALookupServiceBeginA" 
int(_stdcall *_WSALookupServiceBeginA)(IN LPWSAQUERYSETA lpqsRestrictions, IN DWORD dwControlFlags, OUT LPHANDLE lphLookup);
//0x003F 0x000035EF "WSALookupServiceBeginW" 
int(_stdcall *_WSALookupServiceBeginW)(IN LPWSAQUERYSETW lpqsRestrictions, IN DWORD dwControlFlags, OUT LPHANDLE lphLookup);
//0x0040 0x0000350E "WSALookupServiceEnd" 
int(_stdcall *_WSALookupServiceEnd)(IN HANDLE hLookup);
//0x0041 0x00005A8D "WSALookupServiceNextA" 
int(_stdcall *_WSALookupServiceNextA)(IN HANDLE hLookup, IN DWORD dwControlFlags, IN OUT LPDWORD lpdwBufferLength, OUT LPWSAQUERYSETA lpqsResults);
//0x0042 0x00003181 "WSALookupServiceNextW" 
int(_stdcall *_WSALookupServiceNextW)(IN HANDLE hLookup, IN DWORD dwControlFlags, IN OUT LPDWORD lpdwBufferLength, OUT LPWSAQUERYSETW lpqsResults);
//0x0043 0x00005086 "WSANSPIoctl" 
int(_stdcall *_WSANSPIoctl)(IN HANDLE hLookup, IN DWORD dwControlCode, IN LPVOID lpvInBuffer, IN DWORD cbInBuffer, OUT LPVOID lpvOutBuffer, IN DWORD cbOutBuffer, OUT LPDWORD lpcbBytesReturned, IN LPWSACOMPLETION lpCompletion);
//0x0044 0x0000BCE1 "WSANtohl" 
int(_stdcall *_WSANtohl)(SOCKET s, u_long netlong, u_long FAR *lphostlong);
//0x0045 0x0000BDC9 "WSANtohs" 
int(_stdcall *_WSANtohs)(SOCKET s, u_short netshort, u_short FAR *lphostshort);
//0x0046 0x00008C20 "WSAProviderConfigChange" 
int(_stdcall *_WSAProviderConfigChange)(IN OUT LPHANDLE lpNotificationHandle, IN LPWSAOVERLAPPED lpOverlapped, IN LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
//0x0047 0x00004CB5 "WSARecv" 
int(_stdcall *_WSARecv)(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
//0x0048 0x0000F5EE "WSARecvDisconnect" 
int(_stdcall *_WSARecvDisconnect)(SOCKET s, LPWSABUF lpInboundDisconnectData);
//0x0049 0x0000F66A "WSARecvFrom" 
int(_stdcall *_WSARecvFrom)(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, struct sockaddr FAR *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
//0x004A 0x0000FCB4 "WSARemoveServiceClass" 
int(_stdcall *_WSARemoveServiceClass)(IN LPGUID lpServiceClassId);
//0x004B 0x00002C80 "WSAResetEvent" 
BOOL(_stdcall *_WSAResetEvent)(WSAEVENT hEvent);
//0x004C 0x000068FA "WSASend" 
int(_stdcall *_WSASend)(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
//0x004D 0x00010A22 "WSASendDisconnect" 
int(_stdcall *_WSASendDisconnect)(SOCKET s, LPWSABUF lpOutboundDisconnectData);
//0x004E 0x00010AAD "WSASendTo" 
int(_stdcall *_WSASendTo)(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr FAR *lpTo, int iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
//0x004F 0x00002C91 "WSASetEvent" 
BOOL(_stdcall *_WSASetEvent)(WSAEVENT hEvent);
//0x0050 0x000104D2 "WSASetServiceA" 
int(_stdcall *_WSASetServiceA)(IN LPWSAQUERYSETA lpqsRegInfo, IN WSAESETSERVICEOP essoperation, IN DWORD dwControlFlags);
//0x0051 0x00010281 "WSASetServiceW" 
int(_stdcall *_WSASetServiceW)(IN LPWSAQUERYSETW lpqsRegInfo, IN WSAESETSERVICEOP essoperation, IN DWORD dwControlFlags);
//0x0052 0x00008B6A "WSASocketA" 
SOCKET(_stdcall *_WSASocketA)(int af, int type, int protocol, LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP g, DWORD dwFlags);
//0x0053 0x0000404E "WSASocketW" 
SOCKET(_stdcall *_WSASocketW)(int af, int type, int protocol, LPWSAPROTOCOL_INFOW lpProtocolInfo, GROUP g, DWORD dwFlags);
//0x0054 0x00009765 "WSAStringToAddressA" 
int(_stdcall *_WSAStringToAddressA)(IN LPSTR AddressString, IN int AddressFamily, IN LPWSAPROTOCOL_INFOA lpProtocolInfo, IN OUT LPSOCKADDR lpAddress, IN OUT LPINT lpAddressLength);
//0x0055 0x0000281E "WSAStringToAddressW" 
int(_stdcall *_WSAStringToAddressW)(IN LPWSTR AddressString, IN int AddressFamily, IN LPWSAPROTOCOL_INFOW lpProtocolInfo, IN OUT LPSOCKADDR lpAddress, IN OUT LPINT lpAddressLength);
//0x0056 0x00002C6F "WSAWaitForMultipleEvents" 
DWORD(_stdcall *_WSAWaitForMultipleEvents)(DWORD cEvents, const WSAEVENT FAR *lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable);
//0x0057 0x000119C9 "WSCDeinstallProvider" 
int(_stdcall *_WSCDeinstallProvider)(LPGUID lpProviderId, LPINT lpErrno);
//0x0058 0x0000EE59 "WSCEnableNSProvider" 
int(_stdcall *_WSCEnableNSProvider)(LPGUID lpProviderId, BOOL fEnable);
//0x0059 0x00007B61 "WSCEnumProtocols" 
int(_stdcall *_WSCEnumProtocols)(LPINT lpiProtocols, LPWSAPROTOCOL_INFOW lpProtocolBuffer, LPDWORD lpdwBufferLength, LPINT lpErrno);
//0x005A 0x00008E27 "WSCGetProviderPath" 
int(_stdcall *_WSCGetProviderPath)(LPGUID lpProviderId, WCHAR FAR *lpszProviderDllPath, LPINT lpProviderDllPathLen, LPINT lpErrno);
//0x005B 0x0000F14D "WSCInstallNameSpace" 
int(_stdcall *_WSCInstallNameSpace)(LPWSTR lpszIdentifier, LPWSTR lpszPathName, DWORD dwNameSpace, DWORD dwVersion, LPGUID lpProviderId);
//0x005C 0x00011665 "WSCInstallProvider" 
int(_stdcall *_WSCInstallProvider)(LPGUID lpProviderId, const WCHAR FAR *lpszProviderDllPath, const LPWSAPROTOCOL_INFOW lpProtocolInfoList, DWORD dwNumberOfEntries, LPINT lpErrno);
//0x005D 0x0000F319 "WSCUnInstallNameSpace" 
int(_stdcall *_WSCUnInstallNameSpace)(LPGUID lpProviderId);
//0x005E 0x00011291 "WSCUpdateProvider" 
int(_stdcall *_WSCUpdateProvider)(IN LPGUID lpProviderId, IN const WCHAR FAR *lpszProviderDllPath, IN const LPWSAPROTOCOL_INFOW lpProtocolInfoList, IN DWORD dwNumberOfEntries, OUT LPINT lpErrno);
//0x005F 0x0000F019 "WSCWriteNameSpaceOrder" 
int(_stdcall *_WSCWriteNameSpaceOrder)(IN LPGUID lpProviderId, IN DWORD dwNumberOfEntries);
//0x0060 0x00011531 "WSCWriteProviderOrder" 
int(_stdcall *_WSCWriteProviderOrder)(IN LPDWORD lpwdCatalogEntryId, IN DWORD dwNumberOfEntries);
//0x0061 0x00002B0B "freeaddrinfo" 
void(_stdcall *_freeaddrinfo)(IN LPADDRINFO pAddrInfo);
//0x0062 0x00002A6F "getaddrinfo" 
int(_stdcall *_getaddrinfo)(IN const char FAR *nodename, IN const char FAR *servname, IN const struct addrinfo FAR *hints, OUT struct addrinfo FAR *FAR *res);
//0x0063 0x0000C689 "getnameinfo" 
int(_stdcall *_getnameinfo)(IN const struct sockaddr FAR *sa, IN socklen_t salen, OUT char FAR *host, IN DWORD hostlen, OUT char FAR *serv, IN DWORD servlen, IN int flags);
//0x0065 0x00010991 "WSAAsyncSelect" 
int(_stdcall *_WSAAsyncSelect) (SOCKET, HWND, u_int, long);
//0x0066 0x0000EA43 "WSAAsyncGetHostByAddr" 
HANDLE(_stdcall *_WSAAsyncGetHostByAddr)(HWND hWnd, u_int wMsg, const char FAR *addr, int len, int type, char FAR *buf, int buflen);
//0x0067 0x0000E99D "WSAAsyncGetHostByName" 
HANDLE(_stdcall *_WSAAsyncGetHostByName)(HWND, u_int, const char FAR *, char FAR *, int);
//0x0068 0x0000E2C3 "WSAAsyncGetProtoByNumber" 
HANDLE(_stdcall *_WSAAsyncGetProtoByNumber)(HWND hWnd, u_int wMsg, int number, char FAR *buf, int buflen);
//0x0069 0x0000EAED "WSAAsyncGetProtoByName" 
HANDLE(_stdcall *_WSAAsyncGetProtoByName)(HWND hWnd, u_int wMsg, const char FAR *name, char FAR *buf, int buflen);
//0x006A 0x0000E913 "WSAAsyncGetServByPort" 
HANDLE(_stdcall *_WSAAsyncGetServByPort)(HWND hWnd, u_int wMsg, int port, const char FAR *proto, char FAR *buf, int buflen);
//0x006B 0x0000E867 "WSAAsyncGetServByName" 
HANDLE(_stdcall *_WSAAsyncGetServByName)(HWND hWnd, u_int wMsg, const char FAR *name, const char FAR *proto, char FAR *buf, int buflen);
//0x006C 0x0000E347 "WSACancelAsyncRequest" 
int(_stdcall *_WSACancelAsyncRequest)(HANDLE hAsyncTaskHandle);
//0x006D 0x0000D4AC "WSASetBlockingHook" 
FARPROC(_stdcall *_WSASetBlockingHook)(FARPROC lpBlockFunc);
//0x006E 0x0000D508 "WSAUnhookBlockingHook" 
int(_stdcall *_WSAUnhookBlockingHook)(void);
//0x006F 0x00003CCE "WSAGetLastError" 
int(_stdcall *_WSAGetLastError) ();
//0x0070 0x00002A5E "WSASetLastError" 
void(_stdcall *_WSASetLastError)(int iError);
//0x0071 0x0000D40A "WSACancelBlockingCall" 
int(_stdcall *_WSACancelBlockingCall)(void);
//0x0072 0x0000D459 "WSAIsBlocking" 
int(_stdcall *_WSAIsBlocking)(void);
//0x0073 0x00006A55 "WSAStartup" 
int(_stdcall *_WSAStartup) (WORD, LPWSADATA);
//0x0074 0x00003FED "WSACleanup" 
int(_stdcall *_WSACleanup) ();
//0x0097 0x00003F7B "__WSAFDIsSet" 
int(_stdcall *___WSAFDIsSet) (SOCKET, fd_set FAR *);
//0x01F4 0x0001211D "WEP" 
int(_stdcall *_WEP)(int nParam);


int _stdcall __recv(SOCKET s, char FAR * buf, int len, int flags) {
	Log("Buffer = %s", buf);
	return _recv(s, buf, len, flags);
}

BOOL InitInstance(void) {
	char winpath[MAX_PATH];

	Log("Started dll\n");

	if (iWS232) return(TRUE);
	//strcpy(winpath, "WS2_32.DLL");
	GetSystemDirectory(winpath, sizeof(winpath));
	strcat_s(winpath, "\\WS2_32.DLL");

	iWS232 = LoadLibrary(winpath);
	
	Log("LoadedLibrary dll\n");

	if (!iWS232) return(FALSE);
	
	Log("Doing next part dll\n");

	//0x0001  0x00011040 "accept"
	aWS232 = GetProcAddress(iWS232, "accept");
	_accept = (int(_stdcall * ) (SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen))aWS232;
	//0x0002  0x00004480 "bind"
	aWS232 = GetProcAddress(iWS232, "bind");
	_bind = (int(_stdcall * )(SOCKET, const struct sockaddr *, int))aWS232;
	//0x0003  0x00003E2B "closesocket"
	aWS232 = GetProcAddress(iWS232, "closesocket");
	_closesocket = (int(_stdcall * )(SOCKET))aWS232;
	//0x0004  0x00004A07 "connect"
	aWS232 = GetProcAddress(iWS232, "connect");
	_connect = (int(_stdcall * )(SOCKET, const struct sockaddr *, int))aWS232;
	//0x0005  0x00010B68 "getpeername"
	aWS232 = GetProcAddress(iWS232, "getpeername");
	_getpeername = (int(_stdcall * )(SOCKET s, struct sockaddr FAR *name, int FAR *namelen))aWS232;
	//0x0006  0x00003D10 "getsockname"
	aWS232 = GetProcAddress(iWS232, "getsockname");
	_getsockname = (int(_stdcall * )(SOCKET, struct sockaddr *, int *))aWS232;
	//0x0007  0x000046EA "getsockopt"
	aWS232 = GetProcAddress(iWS232, "getsockopt");
	_getsockopt = (int(_stdcall * )(SOCKET, int, int, char *, int *))aWS232;
	//0x0008  0x00002EAD "htonl"
	aWS232 = GetProcAddress(iWS232, "htonl");
	_htonl = (u_long(_stdcall *)(u_long))aWS232;
	//0x0009  0x00002E53 "htons"
	aWS232 = GetProcAddress(iWS232, "htons");
	_htons = (u_short(_stdcall *)(u_short))aWS232;
	//0x000A  0x00003F50 "ioctlsocket"
	aWS232 = GetProcAddress(iWS232, "ioctlsocket");
	_ioctlsocket = (int(_stdcall * )(SOCKET, long, u_long FAR *))aWS232;
	//0x000B  0x00002EE1 "inet_addr"
	aWS232 = GetProcAddress(iWS232, "inet_addr");
	_inet_addr = (unsigned long(_stdcall *)(const char FAR *))aWS232;
	//0x000C  0x000045C1 "inet_ntoa"
	aWS232 = GetProcAddress(iWS232, "inet_ntoa");
	_inet_ntoa = (char *  (_stdcall *)(struct in_addr))aWS232;
	//0x000D  0x00008CD3 "listen"
	aWS232 = GetProcAddress(iWS232, "listen");
	_listen = (int(_stdcall * )(SOCKET s, int backlog))aWS232;
	//0x000E  0x00002EAD "ntohl"
	aWS232 = GetProcAddress(iWS232, "ntohl");
	_ntohl = (u_long(_stdcall * )(u_long netlong))aWS232;
	//0x000F  0x00002E53 "ntohs"
	aWS232 = GetProcAddress(iWS232, "ntohs");
	_ntohs = (u_short(_stdcall *)(u_short))aWS232;
	//0x0010  0x0000676F "recv"
	aWS232 = GetProcAddress(iWS232, "recv");
	_recv = (int(_stdcall * )(SOCKET, char FAR *, int, int))aWS232;
	//0x0011  0x00002FF7 "recvfrom"
	aWS232 = GetProcAddress(iWS232, "recvfrom");
	_recvfrom = (int(_stdcall * )(SOCKET s, char FAR *buf, int len, int flags, struct sockaddr FAR *from, int FAR *fromlen))aWS232;
	//0x0012  0x000030A8 "select"
	aWS232 = GetProcAddress(iWS232, "select");
	_select = (int(_stdcall * )(int, fd_set FAR *, fd_set FAR *, fd_set FAR *, const struct timeval FAR *))aWS232;
	//0x0013  0x00004C27 "send"
	aWS232 = GetProcAddress(iWS232, "send");
	_send = (int(_stdcall * )(SOCKET, const char *, int, int))aWS232;
	//0x0014  0x00002F51 "sendto"
	aWS232 = GetProcAddress(iWS232, "sendto");
	_sendto = (int(_stdcall *)(SOCKET s, const char FAR *buf, int len, int flags, const struct sockaddr FAR *to, int tolen))aWS232;
	//0x0015  0x00004521 "setsockopt"
	aWS232 = GetProcAddress(iWS232, "setsockopt");
	_setsockopt = (int(_stdcall * )(SOCKET, int, int, const char *, int))aWS232;
	//0x0016  0x00010BF6 "shutdown"
	aWS232 = GetProcAddress(iWS232, "shutdown");
	_shutdown = (int(_stdcall * ) (SOCKET s, int how))aWS232;
	//0x0017  0x00004211 "socket"
	aWS232 = GetProcAddress(iWS232, "socket");
	_socket = (SOCKET(_stdcall * )(int, int, int))aWS232;
	//0x0018  0x00002899 "GetAddrInfoW"
	aWS232 = GetProcAddress(iWS232, "GetAddrInfoW");
	_GetAddrInfoW = (int(_stdcall * )(IN PCWSTR, IN PCWSTR, IN const ADDRINFOW *, OUT PADDRINFOW *))aWS232;
	//0x0019  0x0000C504 "GetNameInfoW"
	aWS232 = GetProcAddress(iWS232, "GetNameInfoW");
	_GetNameInfoW = (int(_stdcall * )(IN const SOCKADDR *, IN socklen_t, OUT PWCHAR, IN DWORD, OUT PWCHAR, IN DWORD, IN int))aWS232;
	//0x001A  0x00011EE1 "WSApSetPostRoutine"

	//0x001B  0x00002B0B "FreeAddrInfoW"
	aWS232 = GetProcAddress(iWS232, "FreeAddrInfoW");
	_FreeAddrInfoW = (void(_stdcall *)(IN  PADDRINFOW))aWS232;
	//0x001C  0x00011CBF "WPUCompleteOverlappedRequest"
	aWS232 = GetProcAddress(iWS232, "WPUCompleteOverlappedRequest");
	_WPUCompleteOverlappedRequest = (int(_stdcall * )(SOCKET, LPWSAOVERLAPPED, DWORD, DWORD, LPINT))aWS232;
	//0x001D  0x00010DC1 "WSAAccept"
	aWS232 = GetProcAddress(iWS232, "WSAAccept");
	_WSAAccept = (SOCKET(_stdcall * )(SOCKET, struct sockaddr FAR *, LPINT, LPCONDITIONPROC, DWORD))aWS232;
	//0x001E  0x00009462 "WSAAddressToStringA"
	aWS232 = GetProcAddress(iWS232, "WSAAddressToStringA");
	_WSAAddressToStringA = (int(_stdcall * )(IN LPSOCKADDR, IN DWORD, IN LPWSAPROTOCOL_INFOA, IN OUT LPSTR, IN OUT LPDWORD))aWS232;
	//0x001F  0x00006979 "WSAAddressToStringW"
	aWS232 = GetProcAddress(iWS232, "WSAAddressToStringA");
	_WSAAddressToStringW = (int(_stdcall * )(IN LPSOCKADDR, IN DWORD, IN LPWSAPROTOCOL_INFOW, IN OUT LPWSTR, IN OUT LPDWORD))aWS232;
	//0x0020  0x000065E8 "WSACloseEvent"
	aWS232 = GetProcAddress(iWS232, "WSAAddressToStringA");
	_WSACloseEvent = (BOOL(_stdcall *)(WSAEVENT))aWS232;
	//0x0021  0x00010C81 "WSAConnect"
	aWS232 = GetProcAddress(iWS232, "WSAConnect");
	_WSAConnect = (int(_stdcall * )(SOCKET, const struct sockaddr FAR *, int, LPWSABUF, LPWSABUF, LPQOS, LPQOS))aWS232;
	//0x0022  0x0000655D "WSACreateEvent"
	aWS232 = GetProcAddress(iWS232, "WSACreateEvent");
	_WSACreateEvent = (WSAEVENT(_stdcall * )(void))aWS232;
	//0x0023  0x0000DB9A "WSADuplicateSocketA"
	aWS232 = GetProcAddress(iWS232, "WSADuplicateSocketA");
	_WSADuplicateSocketA = (int(_stdcall * )(SOCKET, DWORD, LPWSAPROTOCOL_INFOA))aWS232;
	//0x0024  0x0000DB0C "WSADuplicateSocketW"
	aWS232 = GetProcAddress(iWS232, "WSADuplicateSocketW");
	_WSADuplicateSocketW = (int(_stdcall * )(SOCKET, DWORD, LPWSAPROTOCOL_INFOW))aWS232;
	//0x0025  0x0000F9E1 "WSAEnumNameSpaceProvidersA"
	aWS232 = GetProcAddress(iWS232, "WSAEnumNameSpaceProvidersA");
	_WSAEnumNameSpaceProvidersA = (int(_stdcall * )(IN OUT LPDWORD, IN LPWSANAMESPACE_INFOA))aWS232;
	//0x0026  0x0000FA41 "WSAEnumNameSpaceProvidersW"
	aWS232 = GetProcAddress(iWS232, "WSAEnumNameSpaceProvidersW");
	_WSAEnumNameSpaceProvidersW = (int(_stdcall * )(IN OUT LPDWORD, IN LPWSANAMESPACE_INFOW))aWS232;
	//0x0027  0x0000657D "WSAEnumNetworkEvents"
	aWS232 = GetProcAddress(iWS232, "WSAEnumNetworkEvents");
	_WSAEnumNetworkEvents = (int(_stdcall * )(SOCKET, WSAEVENT, LPWSANETWORKEVENTS))aWS232;
	//0x0028  0x0000DC5F "WSAEnumProtocolsA"
	aWS232 = GetProcAddress(iWS232, "WSAEnumProtocolsA");
	_WSAEnumProtocolsA = (int(_stdcall * )(LPINT, LPWSAPROTOCOL_INFOA, LPDWORD))aWS232;
	//0x0029  0x0000888C "WSAEnumProtocolsW"
	aWS232 = GetProcAddress(iWS232, "WSAEnumProtocolsW");
	_WSAEnumProtocolsW = (int(_stdcall * )(LPINT, LPWSAPROTOCOL_INFOW, LPDWORD))aWS232;
	//0x002A  0x000064D9 "WSAEventSelect"
	aWS232 = GetProcAddress(iWS232, "WSAEventSelect");
	_WSAEventSelect = (int(_stdcall * )(SOCKET, WSAEVENT, long))aWS232;
	//0x002B  0x00010D1B "WSAGetOverlappedResult"
	aWS232 = GetProcAddress(iWS232, "WSAGetOverlappedResult");
	_WSAGetOverlappedResult = (BOOL(_stdcall *)(SOCKET, LPWSAOVERLAPPED, LPDWORD, BOOL, LPDWORD))aWS232;
	//0x002C  0x0000F470 "WSAGetQOSByName"
	aWS232 = GetProcAddress(iWS232, "WSAGetQOSByName");
	_WSAGetQOSByName = (BOOL(_stdcall *)(SOCKET, LPWSABUF, LPQOS))aWS232;
	//0x002D  0x00010440 "WSAGetServiceClassInfoA"
	aWS232 = GetProcAddress(iWS232, "WSAGetServiceClassInfoA");
	_WSAGetServiceClassInfoA = (int(_stdcall * )(IN  LPGUID, IN  LPGUID, IN OUT LPDWORD, OUT LPWSASERVICECLASSINFOA))aWS232;
	//0x002E  0x00010159 "WSAGetServiceClassInfoW"
	aWS232 = GetProcAddress(iWS232, "WSAGetServiceClassInfoW");
	_WSAGetServiceClassInfoW = (int(_stdcall * )(IN  LPGUID, IN  LPGUID, IN OUT LPDWORD, OUT LPWSASERVICECLASSINFOW))aWS232;
	//0x002F  0x0000FD9F "WSAGetServiceClassNameByClassIdA"
	aWS232 = GetProcAddress(iWS232, "WSAGetServiceClassNameByClassIdA");
	_WSAGetServiceClassNameByClassIdA = (int(_stdcall * )(IN LPGUID, OUT LPSTR, IN OUT  LPDWORD))aWS232;
	//0x0030  0x0000FF81 "WSAGetServiceClassNameByClassIdW"
	aWS232 = GetProcAddress(iWS232, "WSAGetServiceClassNameByClassIdW");
	_WSAGetServiceClassNameByClassIdW = (int(_stdcall * )(IN LPGUID, OUT LPWSTR, IN OUT  LPDWORD))aWS232;
	//0x0031  0x0000BCE1 "WSAHtonl"
	aWS232 = GetProcAddress(iWS232, "WSAHtonl");
	_WSAHtonl = (int(_stdcall * )(SOCKET, u_long, u_long FAR *))aWS232;
	//0x0032  0x0000BDC9 "WSAHtons"
	aWS232 = GetProcAddress(iWS232, "WSAHtons");
	_WSAHtons = (int(_stdcall * )(SOCKET, u_short, u_short FAR *))aWS232;
	//0x0033  0x0000E491 "gethostbyaddr"
	aWS232 = GetProcAddress(iWS232, "gethostbyaddr");
	_gethostbyaddr = (struct hostent * (_stdcall *)(const char FAR *addr, int len, int type))aWS232;
	//0x0034  0x00005355 "gethostbyname"
	aWS232 = GetProcAddress(iWS232, "gethostbyname");
	_gethostbyname = (struct hostent * (_stdcall *)(const char FAR *))aWS232;
	//0x0035  0x0000E18E "getprotobyname"
	aWS232 = GetProcAddress(iWS232, "getprotobyname");
	_getprotobyname = (struct protoent * (_stdcall *)(const char FAR *name))aWS232;
	//0x0036  0x0000E0CC "getprotobynumber"
	aWS232 = GetProcAddress(iWS232, "getprotobynumber");
	_getprotobynumber = (struct protoent * (_stdcall *)(int proto))aWS232;
	//0x0037  0x0000E703 "getservbyname"
	aWS232 = GetProcAddress(iWS232, "getservbyname");
	_getservbyname = (struct servent * (_stdcall *)(const char FAR *name, const char FAR *proto))aWS232;
	//0x0038  0x0000E5B5 "getservbyport"
	aWS232 = GetProcAddress(iWS232, "getservbyport");
	_getservbyport = (struct servent * (_stdcall *)(int port, const char FAR *proto))aWS232;
	//0x0039  0x00005449 "gethostname"
	aWS232 = GetProcAddress(iWS232, "gethostname");
	_gethostname = (int(_stdcall * )(char FAR *, int))aWS232;
	//0x003A  0x000103C1 "WSAInstallServiceClassA"
	aWS232 = GetProcAddress(iWS232, "WSAInstallServiceClassA");
	_WSAInstallServiceClassA = (int(_stdcall * )(IN  LPWSASERVICECLASSINFOA))aWS232;
	//0x003B  0x0000FBC9 "WSAInstallServiceClassW"
	aWS232 = GetProcAddress(iWS232, "WSAInstallServiceClassW");
	_WSAInstallServiceClassW = (int(_stdcall * )(IN  LPWSASERVICECLASSINFOW))aWS232;
	//0x003C  0x00003EC0 "WSAIoctl"
	aWS232 = GetProcAddress(iWS232, "WSAIoctl");
	_WSAIoctl = (int(_stdcall * )(SOCKET, DWORD, LPVOID, DWORD, LPVOID, DWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE))aWS232;
	//0x003D  0x00010F70 "WSAJoinLeaf"
	aWS232 = GetProcAddress(iWS232, "WSAJoinLeaf");
	_WSAJoinLeaf = (SOCKET(_stdcall * )(SOCKET, const struct sockaddr FAR *, int, LPWSABUF, LPWSABUF, LPQOS, LPQOS, DWORD))aWS232;
	//0x003E  0x0000562A "WSALookupServiceBeginA"
	aWS232 = GetProcAddress(iWS232, "WSALookupServiceBeginA");
	_WSALookupServiceBeginA = (int(_stdcall * )(IN  LPWSAQUERYSETA, IN  DWORD, OUT LPHANDLE))aWS232;
	//0x003F  0x000035EF "WSALookupServiceBeginW"
	aWS232 = GetProcAddress(iWS232, "WSALookupServiceBeginW");
	_WSALookupServiceBeginW = (int(_stdcall * )(IN  LPWSAQUERYSETW, IN  DWORD, OUT LPHANDLE))aWS232;
	//0x0040  0x0000350E "WSALookupServiceEnd"
	aWS232 = GetProcAddress(iWS232, "WSALookupServiceEnd");
	_WSALookupServiceEnd = (int(_stdcall * )(IN HANDLE))aWS232;
	//0x0041  0x00005A8D "WSALookupServiceNextA"
	aWS232 = GetProcAddress(iWS232, "WSALookupServiceNextA");
	_WSALookupServiceNextA = (int(_stdcall * )(IN HANDLE, IN DWORD, IN OUT LPDWORD, OUT    LPWSAQUERYSETA))aWS232;
	//0x0042  0x00003181 "WSALookupServiceNextW"
	aWS232 = GetProcAddress(iWS232, "WSALookupServiceNextW");
	_WSALookupServiceNextW = (int(_stdcall * )(IN HANDLE, IN DWORD, IN OUT LPDWORD, OUT    LPWSAQUERYSETW))aWS232;
	//0x0043  0x00005086 "WSANSPIoctl"
	aWS232 = GetProcAddress(iWS232, "WSANSPIoctl");
	_WSANSPIoctl = (int(_stdcall *)(IN HANDLE hLookup, IN DWORD dwControlCode, IN LPVOID lpvInBuffer, IN DWORD cbInBuffer, OUT LPVOID lpvOutBuffer, IN DWORD cbOutBuffer, OUT LPDWORD lpcbBytesReturned, IN LPWSACOMPLETION lpCompletion))aWS232;
	//0x0044  0x0000BCE1 "WSANtohl"
	aWS232 = GetProcAddress(iWS232, "WSANtohl");
	_WSANtohl = (int(_stdcall * )(SOCKET, u_long, u_long FAR *))aWS232;
	//0x0045  0x0000BDC9 "WSANtohs"
	aWS232 = GetProcAddress(iWS232, "WSANtohs");
	_WSANtohs = (int(_stdcall * )(SOCKET, u_short, u_short FAR *))aWS232;
	//0x0046  0x00008C20 "WSAProviderConfigChange"
	aWS232 = GetProcAddress(iWS232, "WSAProviderConfigChange");
	_WSAProviderConfigChange = (int(_stdcall * )(IN OUT LPHANDLE, IN LPWSAOVERLAPPED, IN LPWSAOVERLAPPED_COMPLETION_ROUTINE))aWS232;
	//0x0047  0x00004CB5 "WSARecv"
	aWS232 = GetProcAddress(iWS232, "WSARecv");
	_WSARecv = (int(_stdcall * )(SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE))aWS232;
	//0x0048  0x0000F5EE "WSARecvDisconnect"
	aWS232 = GetProcAddress(iWS232, "WSARecvDisconnect");
	_WSARecvDisconnect = (int(_stdcall * )(SOCKET, LPWSABUF))aWS232;
	//0x0049  0x0000F66A "WSARecvFrom"
	aWS232 = GetProcAddress(iWS232, "WSARecvFrom");
	_WSARecvFrom = (int(_stdcall * )(SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, struct sockaddr FAR *, LPINT, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE))aWS232;
	//0x004A  0x0000FCB4 "WSARemoveServiceClass"
	aWS232 = GetProcAddress(iWS232, "WSARemoveServiceClass");
	_WSARemoveServiceClass = (int(_stdcall * )(IN  LPGUID))aWS232;
	//0x004B  0x00002C80 "WSAResetEvent"
	aWS232 = GetProcAddress(iWS232, "WSAResetEvent");
	_WSAResetEvent = (BOOL(_stdcall *)(WSAEVENT))aWS232;
	//0x004C  0x000068FA "WSASend"
	aWS232 = GetProcAddress(iWS232, "WSASend");
	_WSASend = (int(_stdcall * )(SOCKET, LPWSABUF, DWORD, LPDWORD, DWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE))aWS232;
	//0x004D  0x00010A22 "WSASendDisconnect"
	aWS232 = GetProcAddress(iWS232, "WSASendDisconnect");
	_WSASendDisconnect = (int(_stdcall * )(SOCKET, LPWSABUF))aWS232;
	//0x004E  0x00010AAD "WSASendTo"
	aWS232 = GetProcAddress(iWS232, "WSASendTo");
	_WSASendTo = (int(_stdcall * )(SOCKET, LPWSABUF, DWORD, LPDWORD, DWORD, const struct sockaddr FAR *, int, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE))aWS232;
	//0x004F  0x00002C91 "WSASetEvent"
	aWS232 = GetProcAddress(iWS232, "WSASetEvent");
	_WSASetEvent = (BOOL(_stdcall *)(WSAEVENT))aWS232;
	//0x0050  0x000104D2 "WSASetServiceA"
	aWS232 = GetProcAddress(iWS232, "WSASetServiceA");
	_WSASetServiceA = (int(_stdcall * )(IN LPWSAQUERYSETA, IN WSAESETSERVICEOP, IN DWORD))aWS232;
	//0x0051  0x00010281 "WSASetServiceW"
	aWS232 = GetProcAddress(iWS232, "WSASetServiceW");
	_WSASetServiceW = (int(_stdcall * )(IN LPWSAQUERYSETW, IN WSAESETSERVICEOP, IN DWORD))aWS232;
	//0x0052  0x00008B6A "WSASocketA"
	aWS232 = GetProcAddress(iWS232, "WSASocketA");
	_WSASocketA = (SOCKET(_stdcall * )(int, int, int, LPWSAPROTOCOL_INFO, GROUP, DWORD))aWS232;
	//0x0053  0x0000404E "WSASocketW"
	aWS232 = GetProcAddress(iWS232, "WSASocketW");
	_WSASocketW = (SOCKET(_stdcall * )(int, int, int, LPWSAPROTOCOL_INFOW, GROUP, DWORD))aWS232;
	//0x0054  0x00009765 "WSAStringToAddressA"
	aWS232 = GetProcAddress(iWS232, "WSAStringToAddressA");
	_WSAStringToAddressA = (int(_stdcall * )(IN LPSTR, IN int, IN LPWSAPROTOCOL_INFOA, IN OUT LPSOCKADDR, IN OUT LPINT))aWS232;
	//0x0055  0x0000281E "WSAStringToAddressW"
	aWS232 = GetProcAddress(iWS232, "WSAStringToAddressW");
	_WSAStringToAddressW = (int(_stdcall * )(IN LPWSTR, IN int, IN LPWSAPROTOCOL_INFOW, IN OUT LPSOCKADDR, IN OUT LPINT))aWS232;
	//0x0056  0x00002C6F "WSAWaitForMultipleEvents"
	aWS232 = GetProcAddress(iWS232, "WSAWaitForMultipleEvents");
	_WSAWaitForMultipleEvents = (DWORD(_stdcall *)(DWORD, const WSAEVENT FAR *, BOOL, DWORD, BOOL))aWS232;
	//0x0057  0x000119C9 "WSCDeinstallProvider"
	aWS232 = GetProcAddress(iWS232, "WSCDeinstallProvider");
	_WSCDeinstallProvider = (int(_stdcall * )(LPGUID, LPINT))aWS232;
	//0x0058  0x0000EE59 "WSCEnableNSProvider"
	aWS232 = GetProcAddress(iWS232, "WSCEnableNSProvider");
	_WSCEnableNSProvider = (int(_stdcall * )(LPGUID, BOOL))aWS232;
	//0x0059  0x00007B61 "WSCEnumProtocols"
	aWS232 = GetProcAddress(iWS232, "WSCEnumProtocols");
	_WSCEnumProtocols = (int(_stdcall * )(LPINT, LPWSAPROTOCOL_INFOW, LPDWORD, LPINT))aWS232;
	//0x005A  0x00008E27 "WSCGetProviderPath"
	aWS232 = GetProcAddress(iWS232, "WSCGetProviderPath");
	_WSCGetProviderPath = (int(_stdcall * )(LPGUID, WCHAR FAR *, LPINT, LPINT))aWS232;
	//0x005B  0x0000F14D "WSCInstallNameSpace"
	aWS232 = GetProcAddress(iWS232, "WSCInstallNameSpace");
	_WSCInstallNameSpace = (int(_stdcall * )(LPWSTR, LPWSTR, DWORD, DWORD, LPGUID))aWS232;
	//0x005C  0x00011665 "WSCInstallProvider"
	aWS232 = GetProcAddress(iWS232, "WSCInstallProvider");
	_WSCInstallProvider = (int(_stdcall * )(LPGUID, const WCHAR FAR *, const LPWSAPROTOCOL_INFOW, DWORD, LPINT))aWS232;
	//0x005D  0x0000F319 "WSCUnInstallNameSpace"
	aWS232 = GetProcAddress(iWS232, "WSCUnInstallNameSpace");
	_WSCUnInstallNameSpace = (int(_stdcall * )(LPGUID))aWS232;
	//0x005E  0x00011291 "WSCUpdateProvider"
	aWS232 = GetProcAddress(iWS232, "WSCUpdateProvider");
	_WSCUpdateProvider = (int(_stdcall * )(IN  LPGUID, IN  const WCHAR FAR *, IN  const LPWSAPROTOCOL_INFOW, IN  DWORD, OUT LPINT))aWS232;
	//0x005F  0x0000F019 "WSCWriteNameSpaceOrder"
	aWS232 = GetProcAddress(iWS232, "WSCWriteNameSpaceOrder");
	_WSCWriteNameSpaceOrder = (int(_stdcall * )(IN LPGUID, IN DWORD))aWS232;
	//0x0060  0x00011531 "WSCWriteProviderOrder"
	aWS232 = GetProcAddress(iWS232, "WSCWriteProviderOrder");
	_WSCWriteProviderOrder = (int(_stdcall * )(IN LPDWORD, IN DWORD))aWS232;
	//0x0061  0x00002B0B "freeaddrinfo"
	aWS232 = GetProcAddress(iWS232, "freeaddrinfo");
	_freeaddrinfo = (void(_stdcall *)(IN  LPADDRINFO))aWS232;
	//0x0062  0x00002A6F "getaddrinfo"
	aWS232 = GetProcAddress(iWS232, "getaddrinfo");
	_getaddrinfo = (int(_stdcall * )(IN const char FAR *, IN const char FAR *, IN const struct addrinfo FAR *, OUT struct addrinfo FAR * FAR *))aWS232;
	//0x0063  0x0000C689 "getnameinfo"
	aWS232 = GetProcAddress(iWS232, "getnameinfo");
	_getnameinfo = (int(_stdcall * )(IN  const struct sockaddr FAR *, IN  socklen_t, OUT char FAR *, IN  DWORD, OUT char FAR *, IN  DWORD, IN  int))aWS232;
	//0x0065  0x00010991 "WSAAsyncSelect"
	aWS232 = GetProcAddress(iWS232, "WSAAsyncSelect");
	_WSAAsyncSelect = (int(_stdcall * )(SOCKET, HWND, u_int, long))aWS232;
	//0x0066  0x0000EA43 "WSAAsyncGetHostByAddr"
	aWS232 = GetProcAddress(iWS232, "WSAAsyncGetHostByAddr");
	_WSAAsyncGetHostByAddr = (HANDLE(_stdcall *)(HWND hWnd, u_int wMsg, const char FAR *addr, int len, int type, char FAR *buf, int buflen))aWS232;
	//0x0067  0x0000E99D "WSAAsyncGetHostByName"
	aWS232 = GetProcAddress(iWS232, "WSAAsyncGetHostByName");
	_WSAAsyncGetHostByName = (HANDLE(_stdcall *)(HWND, u_int, const char FAR *, char FAR *, int))aWS232;
	//0x0068  0x0000E2C3 "WSAAsyncGetProtoByNumber"
	aWS232 = GetProcAddress(iWS232, "WSAAsyncGetProtoByNumber");
	_WSAAsyncGetProtoByNumber = (HANDLE(_stdcall *)(HWND hWnd, u_int wMsg, int number, char FAR *buf, int buflen))aWS232;
	//0x0069  0x0000EAED "WSAAsyncGetProtoByName"
	aWS232 = GetProcAddress(iWS232, "WSAAsyncGetProtoByName");
	_WSAAsyncGetProtoByName = (HANDLE(_stdcall *)(HWND hWnd, u_int wMsg, const char FAR *name, char FAR *buf, int buflen))aWS232;
	//0x006A  0x0000E913 "WSAAsyncGetServByPort"
	aWS232 = GetProcAddress(iWS232, "WSAAsyncGetServByPort");
	_WSAAsyncGetServByPort = (HANDLE(_stdcall *)(HWND hWnd, u_int wMsg, int port, const char FAR *proto, char FAR *buf, int buflen))aWS232;
	//0x006B  0x0000E867 "WSAAsyncGetServByName"
	aWS232 = GetProcAddress(iWS232, "WSAAsyncGetServByName");
	_WSAAsyncGetServByName = (HANDLE(_stdcall *)(HWND hWnd, u_int wMsg, const char FAR *name, const char FAR *proto, char FAR *buf, int buflen))aWS232;
	//0x006C  0x0000E347 "WSACancelAsyncRequest"
	aWS232 = GetProcAddress(iWS232, "WSACancelAsyncRequest");
	_WSACancelAsyncRequest = (int(_stdcall *)(HANDLE hAsyncTaskHandle))aWS232;
	//0x006D  0x0000D4AC "WSASetBlockingHook"
	aWS232 = GetProcAddress(iWS232, "WSASetBlockingHook");
	_WSASetBlockingHook = (FARPROC(_stdcall *)(FARPROC lpBlockFunc))aWS232;
	//0x006E  0x0000D508 "WSAUnhookBlockingHook"
	aWS232 = GetProcAddress(iWS232, "WSAUnhookBlockingHook");
	_WSAUnhookBlockingHook = (int(_stdcall *)(void))aWS232;
	//0x006F  0x00003CCE "WSAGetLastError"
	aWS232 = GetProcAddress(iWS232, "WSAGetLastError");
	_WSAGetLastError = (int(_stdcall * )())aWS232;
	//0x0070  0x00002A5E "WSASetLastError"
	aWS232 = GetProcAddress(iWS232, "WSASetLastError");
	_WSASetLastError = (void(_stdcall *)(int iError))aWS232;
	//0x0071  0x0000D40A "WSACancelBlockingCall"
	aWS232 = GetProcAddress(iWS232, "WSACancelBlockingCall");
	_WSACancelBlockingCall = (int(_stdcall *)(void))aWS232;
	//0x0072  0x0000D459 "WSAIsBlocking"
	aWS232 = GetProcAddress(iWS232, "WSAIsBlocking");
	_WSAIsBlocking = (int(_stdcall *)(void))aWS232;
	//0x0073  0x00006A55 "WSAStartup"
	aWS232 = GetProcAddress(iWS232, "WSAStartup");
	_WSAStartup = (int(_stdcall * )(WORD, LPWSADATA))aWS232;
	//0x0074  0x00003FED "WSACleanup"
	aWS232 = GetProcAddress(iWS232, "WSACleanup");
	_WSACleanup = (int(_stdcall * )())aWS232;
	//0x0097  0x00003F7B "__WSAFDIsSet"
	aWS232 = GetProcAddress(iWS232, "__WSAFDIsSet");
	___WSAFDIsSet = (int(_stdcall * )(SOCKET, fd_set FAR *))aWS232;
	//0x01F4  0x0001211D "WEP"
	aWS232=GetProcAddress(iWS232,"WEP");
	_WEP=(int (_stdcall *)(int))aWS232;
	return(TRUE);
}

void ExitInstance(void) {
	if (iWS232) {
		FreeLibrary(iWS232);
		iWS232 = NULL;
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hinstDLL);
		MessageBox(NULL, "Test", "test", 0);
		InitInstance();
		break;
	}
	case DLL_PROCESS_DETACH: {
		ExitInstance();
		break;
	}
	default: break;
	}
	return(TRUE);
}



