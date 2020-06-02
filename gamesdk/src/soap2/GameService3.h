//
// sproxy.exe generated file
// do not modify this file
//
// Created: 09/12/2005@19:55:2
//

#pragma once


#ifndef _WIN32_WINDOWS
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace GameService3Service
{

struct protocol
{
	BSTR className;
	__int64 id;
	BSTR name;
};

struct game
{
	ATLSOAP_BLOB annotation;
	BSTR author;
	ATLSOAP_BLOB commandLine;
	ATLSOAP_BLOB direction;
	BSTR files;
	ATLSOAP_BLOB game_picture;
	BSTR game_picture_name;
	BSTR game_url;
	__int64 id;
	BSTR name;
	bool playerPingPresent;
	protocol protocol0;
	int publication_date;
	BSTR publisher;
};

struct serverAddress
{
	BSTR host;
	int port;
};

struct rule
{
	ATLSOAP_BLOB key;
	ATLSOAP_BLOB value;
};

struct playerShortInfo
{
	ATLSOAP_BLOB fullName;
	int ping;
	int score;
};

struct serverShortInfo
{
	bool active;
	int currentPlayer;
	__int64 gameID;
	BSTR host;
	__int64 id;
	BSTR map;
	int maxPlayer;
	BSTR name;
	int port;
	BSTR regime;
	int sessionId;
	BSTR sessionName;
	__int64 status;
};

struct serversRoutine
{
	__int64 id;
	BSTR name;
};

struct serverUpdateInfo
{
	bool active;
	int changeStatus;
	__int64 id;
	BSTR map;
	int maxPlayer;
	playerShortInfo *players;
	int __players_nSizeIs;
	int sessionId;
	BSTR sessionName;
};

template <typename TClient = CSoapSocketClientT<> >
class CGameService3ServiceT : 
	public TClient, 
	public CSoapRootHandler
{
protected:

	const _soapmap ** GetFunctionMap();
	const _soapmap ** GetHeaderMap();
	void * GetHeaderValue();
	const wchar_t * GetNamespaceUri();
	const char * GetServiceName();
	const char * GetNamespaceUriA();
	HRESULT CallFunction(
		void *pvParam, 
		const wchar_t *wszLocalName, int cchLocalName,
		size_t nItem);
	HRESULT GetClientReader(ISAXXMLReader **ppReader);

public:

	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		if (ppv == NULL)
		{
			return E_POINTER;
		}

		*ppv = NULL;

		if (InlineIsEqualGUID(riid, IID_IUnknown) ||
			InlineIsEqualGUID(riid, IID_ISAXContentHandler))
		{
			*ppv = static_cast<ISAXContentHandler *>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef()
	{
		return 1;
	}

	ULONG __stdcall Release()
	{
		return 1;
	}

	CGameService3ServiceT(ISAXXMLReader *pReader = NULL)
		:TClient(_T("http://games.ws.services.mtu.ru"))
	{
		SetClient(true);
		SetReader(pReader);
	}
	
	~CGameService3ServiceT()
	{
		Uninitialize();
	}
	
	void Uninitialize()
	{
		UninitializeSOAP();
	}	

	HRESULT getRegisteredServerShortInfos(
		serverShortInfo** getRegisteredServerShortInfosResponse, int* getRegisteredServerShortInfosResponse_nSizeIs
	);

	HRESULT getServerAddressesByGameName(
		BSTR gameName, 
		serverAddress** getServerAddressesByGameNameResponse, int* getServerAddressesByGameNameResponse_nSizeIs
	);

	HRESULT getServerRules(
		__int64 id, 
		rule** getServerRulesResponse, int* getServerRulesResponse_nSizeIs
	);

	HRESULT getServersRoutines(
		serversRoutine** getServersRoutinesResponse, int* getServersRoutinesResponse_nSizeIs
	);

	HRESULT getSupportedGames(
		BSTR gameName, 
		game** getSupportedGamesResponse, int* getSupportedGamesResponse_nSizeIs
	);

	HRESULT getSessionRules(
		__int64 id, 
		int sessionId, 
		rule** getSessionRulesResponse, int* getSessionRulesResponse_nSizeIs
	);

	HRESULT getServerUpdateInfo(
		__int64 id, 
		int sessionId, 
		serverUpdateInfo* getServerUpdateInfoResponse
	);
};

typedef CGameService3ServiceT<> CGameService3Service;

__if_not_exists(__protocol_entries)
{
extern __declspec(selectany) const _soapmapentry __protocol_entries[] =
{
	{ 
		0xF7453A57, 
		"className", 
		L"className", 
		sizeof("className")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(protocol, className),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x00000DED, 
		"id", 
		L"id", 
		sizeof("id")-1, 
		SOAPTYPE_LONG, 
		SOAPFLAG_FIELD, 
		offsetof(protocol, id),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x003DFCC1, 
		"name", 
		L"name", 
		sizeof("name")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(protocol, name),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __protocol_map =
{
	0xAF9A7A92,
	"protocol",
	L"protocol",
	sizeof("protocol")-1,
	sizeof("protocol")-1,
	SOAPMAP_STRUCT,
	__protocol_entries,
	sizeof(protocol),
	3,
	-1,
	SOAPFLAG_NONE,
	0xBEC67B36,
	"urn:BeanService",
	L"urn:BeanService",
	sizeof("urn:BeanService")-1
};
}

__if_not_exists(__game_entries)
{
extern __declspec(selectany) const _soapmapentry __game_entries[] =
{
	{ 
		0x8FF6777B, 
		"annotation", 
		L"annotation", 
		sizeof("annotation")-1, 
		SOAPTYPE_BASE64BINARY, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(game, annotation),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xEACAFD13, 
		"author", 
		L"author", 
		sizeof("author")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(game, author),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xB7184D67, 
		"commandLine", 
		L"commandLine", 
		sizeof("commandLine")-1, 
		SOAPTYPE_BASE64BINARY, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(game, commandLine),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xA1E56D61, 
		"direction", 
		L"direction", 
		sizeof("direction")-1, 
		SOAPTYPE_BASE64BINARY, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(game, direction),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x07713013, 
		"files", 
		L"files", 
		sizeof("files")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(game, files),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x3DBAF4B5, 
		"game_picture", 
		L"game_picture", 
		sizeof("game_picture")-1, 
		SOAPTYPE_BASE64BINARY, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(game, game_picture),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xB9DE4275, 
		"game_picture_name", 
		L"game_picture_name", 
		sizeof("game_picture_name")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(game, game_picture_name),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x3EE9BBEC, 
		"game_url", 
		L"game_url", 
		sizeof("game_url")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(game, game_url),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x00000DED, 
		"id", 
		L"id", 
		sizeof("id")-1, 
		SOAPTYPE_LONG, 
		SOAPFLAG_FIELD, 
		offsetof(game, id),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x003DFCC1, 
		"name", 
		L"name", 
		sizeof("name")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(game, name),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xF2CFEE3C, 
		"playerPingPresent", 
		L"playerPingPresent", 
		sizeof("playerPingPresent")-1, 
		SOAPTYPE_BOOLEAN, 
		SOAPFLAG_FIELD, 
		offsetof(game, playerPingPresent),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xAF9A7A92, 
		"protocol", 
		L"protocol", 
		sizeof("protocol")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_FIELD, 
		offsetof(game, protocol0),
		NULL, 
		&__protocol_map, 
		-1 
	},
	{ 
		0x67AB1B77, 
		"publication_date", 
		L"publication_date", 
		sizeof("publication_date")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(game, publication_date),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x6BED056E, 
		"publisher", 
		L"publisher", 
		sizeof("publisher")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(game, publisher),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __game_map =
{
	0x003A261A,
	"game",
	L"game",
	sizeof("game")-1,
	sizeof("game")-1,
	SOAPMAP_STRUCT,
	__game_entries,
	sizeof(game),
	14,
	-1,
	SOAPFLAG_NONE,
	0xBEC67B36,
	"urn:BeanService",
	L"urn:BeanService",
	sizeof("urn:BeanService")-1
};
}

__if_not_exists(__serverAddress_entries)
{
extern __declspec(selectany) const _soapmapentry __serverAddress_entries[] =
{
	{ 
		0x003AEEDE, 
		"host", 
		L"host", 
		sizeof("host")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(serverAddress, host),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x003F51C5, 
		"port", 
		L"port", 
		sizeof("port")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(serverAddress, port),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __serverAddress_map =
{
	0xDE65385D,
	"serverAddress",
	L"serverAddress",
	sizeof("serverAddress")-1,
	sizeof("serverAddress")-1,
	SOAPMAP_STRUCT,
	__serverAddress_entries,
	sizeof(serverAddress),
	2,
	-1,
	SOAPFLAG_NONE,
	0xBEC67B36,
	"urn:BeanService",
	L"urn:BeanService",
	sizeof("urn:BeanService")-1
};
}

__if_not_exists(__rule_entries)
{
extern __declspec(selectany) const _soapmapentry __rule_entries[] =
{
	{ 
		0x0001D4A9, 
		"key", 
		L"key", 
		sizeof("key")-1, 
		SOAPTYPE_BASE64BINARY, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(rule, key),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x088E571D, 
		"value", 
		L"value", 
		sizeof("value")-1, 
		SOAPTYPE_BASE64BINARY, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(rule, value),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __rule_map =
{
	0x00408338,
	"rule",
	L"rule",
	sizeof("rule")-1,
	sizeof("rule")-1,
	SOAPMAP_STRUCT,
	__rule_entries,
	sizeof(rule),
	2,
	-1,
	SOAPFLAG_NONE,
	0xBEC67B36,
	"urn:BeanService",
	L"urn:BeanService",
	sizeof("urn:BeanService")-1
};
}

__if_not_exists(__playerShortInfo_entries)
{
extern __declspec(selectany) const _soapmapentry __playerShortInfo_entries[] =
{
	{ 
		0x545480D4, 
		"fullName", 
		L"fullName", 
		sizeof("fullName")-1, 
		SOAPTYPE_BASE64BINARY, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(playerShortInfo, fullName),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x003F37AE, 
		"ping", 
		L"ping", 
		sizeof("ping")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(playerShortInfo, ping),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x085932BC, 
		"score", 
		L"score", 
		sizeof("score")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(playerShortInfo, score),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __playerShortInfo_map =
{
	0x408AC629,
	"playerShortInfo",
	L"playerShortInfo",
	sizeof("playerShortInfo")-1,
	sizeof("playerShortInfo")-1,
	SOAPMAP_STRUCT,
	__playerShortInfo_entries,
	sizeof(playerShortInfo),
	3,
	-1,
	SOAPFLAG_NONE,
	0xBEC67B36,
	"urn:BeanService",
	L"urn:BeanService",
	sizeof("urn:BeanService")-1
};
}

__if_not_exists(__serverShortInfo_entries)
{
extern __declspec(selectany) const _soapmapentry __serverShortInfo_entries[] =
{
	{ 
		0xE985491C, 
		"active", 
		L"active", 
		sizeof("active")-1, 
		SOAPTYPE_BOOLEAN, 
		SOAPFLAG_FIELD, 
		offsetof(serverShortInfo, active),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x5E339110, 
		"currentPlayer", 
		L"currentPlayer", 
		sizeof("currentPlayer")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(serverShortInfo, currentPlayer),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xF75C1E47, 
		"gameID", 
		L"gameID", 
		sizeof("gameID")-1, 
		SOAPTYPE_LONG, 
		SOAPFLAG_FIELD, 
		offsetof(serverShortInfo, gameID),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x003AEEDE, 
		"host", 
		L"host", 
		sizeof("host")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(serverShortInfo, host),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x00000DED, 
		"id", 
		L"id", 
		sizeof("id")-1, 
		SOAPTYPE_LONG, 
		SOAPFLAG_FIELD, 
		offsetof(serverShortInfo, id),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x0001DC9E, 
		"map", 
		L"map", 
		sizeof("map")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(serverShortInfo, map),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xF5B08AD3, 
		"maxPlayer", 
		L"maxPlayer", 
		sizeof("maxPlayer")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(serverShortInfo, maxPlayer),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x003DFCC1, 
		"name", 
		L"name", 
		sizeof("name")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(serverShortInfo, name),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x003F51C5, 
		"port", 
		L"port", 
		sizeof("port")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(serverShortInfo, port),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x114A0AB9, 
		"regime", 
		L"regime", 
		sizeof("regime")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(serverShortInfo, regime),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x323513F1, 
		"sessionId", 
		L"sessionId", 
		sizeof("sessionId")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(serverShortInfo, sessionId),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x93CC93C5, 
		"sessionName", 
		L"sessionName", 
		sizeof("sessionName")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(serverShortInfo, sessionName),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x14AB8884, 
		"status", 
		L"status", 
		sizeof("status")-1, 
		SOAPTYPE_LONG, 
		SOAPFLAG_FIELD, 
		offsetof(serverShortInfo, status),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __serverShortInfo_map =
{
	0x3E150573,
	"serverShortInfo",
	L"serverShortInfo",
	sizeof("serverShortInfo")-1,
	sizeof("serverShortInfo")-1,
	SOAPMAP_STRUCT,
	__serverShortInfo_entries,
	sizeof(serverShortInfo),
	13,
	-1,
	SOAPFLAG_NONE,
	0xBEC67B36,
	"urn:BeanService",
	L"urn:BeanService",
	sizeof("urn:BeanService")-1
};
}

__if_not_exists(__serversRoutine_entries)
{
extern __declspec(selectany) const _soapmapentry __serversRoutine_entries[] =
{
	{ 
		0x00000DED, 
		"id", 
		L"id", 
		sizeof("id")-1, 
		SOAPTYPE_LONG, 
		SOAPFLAG_FIELD, 
		offsetof(serversRoutine, id),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x003DFCC1, 
		"name", 
		L"name", 
		sizeof("name")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(serversRoutine, name),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __serversRoutine_map =
{
	0x8036A9B0,
	"serversRoutine",
	L"serversRoutine",
	sizeof("serversRoutine")-1,
	sizeof("serversRoutine")-1,
	SOAPMAP_STRUCT,
	__serversRoutine_entries,
	sizeof(serversRoutine),
	2,
	-1,
	SOAPFLAG_NONE,
	0xBEC67B36,
	"urn:BeanService",
	L"urn:BeanService",
	sizeof("urn:BeanService")-1
};
}

__if_not_exists(__serverUpdateInfo_entries)
{
extern __declspec(selectany) const _soapmapentry __serverUpdateInfo_entries[] =
{
	{ 
		0xE985491C, 
		"active", 
		L"active", 
		sizeof("active")-1, 
		SOAPTYPE_BOOLEAN, 
		SOAPFLAG_FIELD, 
		offsetof(serverUpdateInfo, active),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xE87D5DEA, 
		"changeStatus", 
		L"changeStatus", 
		sizeof("changeStatus")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(serverUpdateInfo, changeStatus),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x00000DED, 
		"id", 
		L"id", 
		sizeof("id")-1, 
		SOAPTYPE_LONG, 
		SOAPFLAG_FIELD, 
		offsetof(serverUpdateInfo, id),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x0001DC9E, 
		"map", 
		L"map", 
		sizeof("map")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(serverUpdateInfo, map),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xF5B08AD3, 
		"maxPlayer", 
		L"maxPlayer", 
		sizeof("maxPlayer")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(serverUpdateInfo, maxPlayer),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xB08717E0, 
		"players", 
		L"players", 
		sizeof("players")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE, 
		offsetof(serverUpdateInfo, players),
		NULL, 
		&__playerShortInfo_map, 
		5+1 
	},
	{ 
		0xB08717E0, 
		"players", 
		L"players", 
		sizeof("players")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD | SOAPFLAG_NOMARSHAL, 
		offsetof(serverUpdateInfo, __players_nSizeIs),
		NULL, 
		NULL, 
		-1
	},
	{ 
		0x323513F1, 
		"sessionId", 
		L"sessionId", 
		sizeof("sessionId")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(serverUpdateInfo, sessionId),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x93CC93C5, 
		"sessionName", 
		L"sessionName", 
		sizeof("sessionName")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(serverUpdateInfo, sessionName),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __serverUpdateInfo_map =
{
	0x6B22B506,
	"serverUpdateInfo",
	L"serverUpdateInfo",
	sizeof("serverUpdateInfo")-1,
	sizeof("serverUpdateInfo")-1,
	SOAPMAP_STRUCT,
	__serverUpdateInfo_entries,
	sizeof(serverUpdateInfo),
	8,
	-1,
	SOAPFLAG_NONE,
	0xBEC67B36,
	"urn:BeanService",
	L"urn:BeanService",
	sizeof("urn:BeanService")-1
};
}

struct __CGameService3Service_getRegisteredServerShortInfos_struct
{
	serverShortInfo *getRegisteredServerShortInfosResponse;
	int __getRegisteredServerShortInfosResponse_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CGameService3Service_getRegisteredServerShortInfos_entries[] =
{

	{
		0x090A2903, 
		"getRegisteredServerShortInfosResponse", 
		L"getRegisteredServerShortInfosResponse", 
		sizeof("getRegisteredServerShortInfosResponse")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CGameService3Service_getRegisteredServerShortInfos_struct, getRegisteredServerShortInfosResponse),
		NULL,
		&__serverShortInfo_map,
		0+1,
	},
	{
		0x090A2903,
		"__getRegisteredServerShortInfosResponse_nSizeIs",
		L"__getRegisteredServerShortInfosResponse_nSizeIs",
		sizeof("__getRegisteredServerShortInfosResponse_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CGameService3Service_getRegisteredServerShortInfos_struct, __getRegisteredServerShortInfosResponse_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CGameService3Service_getRegisteredServerShortInfos_map =
{
	0x2C2FB2D4,
	"getRegisteredServerShortInfos",
	L"getRegisteredServerShortInfos",
	sizeof("getRegisteredServerShortInfos")-1,
	sizeof("getRegisteredServerShortInfos")-1,
	SOAPMAP_FUNC,
	__CGameService3Service_getRegisteredServerShortInfos_entries,
	sizeof(__CGameService3Service_getRegisteredServerShortInfos_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0xAF490AE8,
	"http://games.ws.services.mtu.ru",
	L"http://games.ws.services.mtu.ru",
	sizeof("http://games.ws.services.mtu.ru")-1
};


struct __CGameService3Service_getServerAddressesByGameName_struct
{
	BSTR gameName;
	serverAddress *getServerAddressesByGameNameResponse;
	int __getServerAddressesByGameNameResponse_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CGameService3Service_getServerAddressesByGameName_entries[] =
{

	{
		0x3EE013BB, 
		"gameName", 
		L"gameName", 
		sizeof("gameName")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CGameService3Service_getServerAddressesByGameName_struct, gameName),
		NULL,
		NULL,
		-1,
	},
	{
		0xCEA20F9A, 
		"getServerAddressesByGameNameResponse", 
		L"getServerAddressesByGameNameResponse", 
		sizeof("getServerAddressesByGameNameResponse")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CGameService3Service_getServerAddressesByGameName_struct, getServerAddressesByGameNameResponse),
		NULL,
		&__serverAddress_map,
		1+1,
	},
	{
		0xCEA20F9A,
		"__getServerAddressesByGameNameResponse_nSizeIs",
		L"__getServerAddressesByGameNameResponse_nSizeIs",
		sizeof("__getServerAddressesByGameNameResponse_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CGameService3Service_getServerAddressesByGameName_struct, __getServerAddressesByGameNameResponse_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CGameService3Service_getServerAddressesByGameName_map =
{
	0x5A91F26B,
	"getServerAddressesByGameName",
	L"getServerAddressesByGameName",
	sizeof("getServerAddressesByGameName")-1,
	sizeof("getServerAddressesByGameName")-1,
	SOAPMAP_FUNC,
	__CGameService3Service_getServerAddressesByGameName_entries,
	sizeof(__CGameService3Service_getServerAddressesByGameName_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0xAF490AE8,
	"http://games.ws.services.mtu.ru",
	L"http://games.ws.services.mtu.ru",
	sizeof("http://games.ws.services.mtu.ru")-1
};


struct __CGameService3Service_getServerRules_struct
{
	__int64 id;
	rule *getServerRulesResponse;
	int __getServerRulesResponse_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CGameService3Service_getServerRules_entries[] =
{

	{
		0x00000DED, 
		"id", 
		L"id", 
		sizeof("id")-1, 
		SOAPTYPE_LONG, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CGameService3Service_getServerRules_struct, id),
		NULL,
		NULL,
		-1,
	},
	{
		0xD31EA671, 
		"getServerRulesResponse", 
		L"getServerRulesResponse", 
		sizeof("getServerRulesResponse")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CGameService3Service_getServerRules_struct, getServerRulesResponse),
		NULL,
		&__rule_map,
		1+1,
	},
	{
		0xD31EA671,
		"__getServerRulesResponse_nSizeIs",
		L"__getServerRulesResponse_nSizeIs",
		sizeof("__getServerRulesResponse_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CGameService3Service_getServerRules_struct, __getServerRulesResponse_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CGameService3Service_getServerRules_map =
{
	0x5F4CA242,
	"getServerRules",
	L"getServerRules",
	sizeof("getServerRules")-1,
	sizeof("getServerRules")-1,
	SOAPMAP_FUNC,
	__CGameService3Service_getServerRules_entries,
	sizeof(__CGameService3Service_getServerRules_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0xAF490AE8,
	"http://games.ws.services.mtu.ru",
	L"http://games.ws.services.mtu.ru",
	sizeof("http://games.ws.services.mtu.ru")-1
};


struct __CGameService3Service_getServersRoutines_struct
{
	serversRoutine *getServersRoutinesResponse;
	int __getServersRoutinesResponse_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CGameService3Service_getServersRoutines_entries[] =
{

	{
		0xE9CF0CD2, 
		"getServersRoutinesResponse", 
		L"getServersRoutinesResponse", 
		sizeof("getServersRoutinesResponse")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CGameService3Service_getServersRoutines_struct, getServersRoutinesResponse),
		NULL,
		&__serversRoutine_map,
		0+1,
	},
	{
		0xE9CF0CD2,
		"__getServersRoutinesResponse_nSizeIs",
		L"__getServersRoutinesResponse_nSizeIs",
		sizeof("__getServersRoutinesResponse_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CGameService3Service_getServersRoutines_struct, __getServersRoutinesResponse_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CGameService3Service_getServersRoutines_map =
{
	0x611137A3,
	"getServersRoutines",
	L"getServersRoutines",
	sizeof("getServersRoutines")-1,
	sizeof("getServersRoutines")-1,
	SOAPMAP_FUNC,
	__CGameService3Service_getServersRoutines_entries,
	sizeof(__CGameService3Service_getServersRoutines_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0xAF490AE8,
	"http://games.ws.services.mtu.ru",
	L"http://games.ws.services.mtu.ru",
	sizeof("http://games.ws.services.mtu.ru")-1
};


struct __CGameService3Service_getSupportedGames_struct
{
	BSTR gameName;
	game *getSupportedGamesResponse;
	int __getSupportedGamesResponse_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CGameService3Service_getSupportedGames_entries[] =
{

	{
		0x3EE013BB, 
		"gameName", 
		L"gameName", 
		sizeof("gameName")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CGameService3Service_getSupportedGames_struct, gameName),
		NULL,
		NULL,
		-1,
	},
	{
		0x9EFE7542, 
		"getSupportedGamesResponse", 
		L"getSupportedGamesResponse", 
		sizeof("getSupportedGamesResponse")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CGameService3Service_getSupportedGames_struct, getSupportedGamesResponse),
		NULL,
		&__game_map,
		1+1,
	},
	{
		0x9EFE7542,
		"__getSupportedGamesResponse_nSizeIs",
		L"__getSupportedGamesResponse_nSizeIs",
		sizeof("__getSupportedGamesResponse_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CGameService3Service_getSupportedGames_struct, __getSupportedGamesResponse_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CGameService3Service_getSupportedGames_map =
{
	0x1D573013,
	"getSupportedGames",
	L"getSupportedGames",
	sizeof("getSupportedGames")-1,
	sizeof("getSupportedGames")-1,
	SOAPMAP_FUNC,
	__CGameService3Service_getSupportedGames_entries,
	sizeof(__CGameService3Service_getSupportedGames_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0xAF490AE8,
	"http://games.ws.services.mtu.ru",
	L"http://games.ws.services.mtu.ru",
	sizeof("http://games.ws.services.mtu.ru")-1
};


struct __CGameService3Service_getSessionRules_struct
{
	__int64 id;
	int sessionId;
	rule *getSessionRulesResponse;
	int __getSessionRulesResponse_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CGameService3Service_getSessionRules_entries[] =
{

	{
		0x00000DED, 
		"id", 
		L"id", 
		sizeof("id")-1, 
		SOAPTYPE_LONG, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CGameService3Service_getSessionRules_struct, id),
		NULL,
		NULL,
		-1,
	},
	{
		0x323513F1, 
		"sessionId", 
		L"sessionId", 
		sizeof("sessionId")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CGameService3Service_getSessionRules_struct, sessionId),
		NULL,
		NULL,
		-1,
	},
	{
		0xC6900ADE, 
		"getSessionRulesResponse", 
		L"getSessionRulesResponse", 
		sizeof("getSessionRulesResponse")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CGameService3Service_getSessionRules_struct, getSessionRulesResponse),
		NULL,
		&__rule_map,
		2+1,
	},
	{
		0xC6900ADE,
		"__getSessionRulesResponse_nSizeIs",
		L"__getSessionRulesResponse_nSizeIs",
		sizeof("__getSessionRulesResponse_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CGameService3Service_getSessionRules_struct, __getSessionRulesResponse_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CGameService3Service_getSessionRules_map =
{
	0xF86AE9AF,
	"getSessionRules",
	L"getSessionRules",
	sizeof("getSessionRules")-1,
	sizeof("getSessionRules")-1,
	SOAPMAP_FUNC,
	__CGameService3Service_getSessionRules_entries,
	sizeof(__CGameService3Service_getSessionRules_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0xAF490AE8,
	"http://games.ws.services.mtu.ru",
	L"http://games.ws.services.mtu.ru",
	sizeof("http://games.ws.services.mtu.ru")-1
};


struct __CGameService3Service_getServerUpdateInfo_struct
{
	__int64 id;
	int sessionId;
	serverUpdateInfo getServerUpdateInfoResponse;
};

extern __declspec(selectany) const _soapmapentry __CGameService3Service_getServerUpdateInfo_entries[] =
{

	{
		0x00000DED, 
		"id", 
		L"id", 
		sizeof("id")-1, 
		SOAPTYPE_LONG, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CGameService3Service_getServerUpdateInfo_struct, id),
		NULL,
		NULL,
		-1,
	},
	{
		0x323513F1, 
		"sessionId", 
		L"sessionId", 
		sizeof("sessionId")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CGameService3Service_getServerUpdateInfo_struct, sessionId),
		NULL,
		NULL,
		-1,
	},
	{
		0xD06D04B5, 
		"getServerUpdateInfoResponse", 
		L"getServerUpdateInfoResponse", 
		sizeof("getServerUpdateInfoResponse")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CGameService3Service_getServerUpdateInfo_struct, getServerUpdateInfoResponse),
		NULL,
		&__serverUpdateInfo_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CGameService3Service_getServerUpdateInfo_map =
{
	0x85D2FC86,
	"getServerUpdateInfo",
	L"getServerUpdateInfo",
	sizeof("getServerUpdateInfo")-1,
	sizeof("getServerUpdateInfo")-1,
	SOAPMAP_FUNC,
	__CGameService3Service_getServerUpdateInfo_entries,
	sizeof(__CGameService3Service_getServerUpdateInfo_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0xAF490AE8,
	"http://games.ws.services.mtu.ru",
	L"http://games.ws.services.mtu.ru",
	sizeof("http://games.ws.services.mtu.ru")-1
};

extern __declspec(selectany) const _soapmap * __CGameService3Service_funcs[] =
{
	&__CGameService3Service_getRegisteredServerShortInfos_map,
	&__CGameService3Service_getServerAddressesByGameName_map,
	&__CGameService3Service_getServerRules_map,
	&__CGameService3Service_getServersRoutines_map,
	&__CGameService3Service_getSupportedGames_map,
	&__CGameService3Service_getSessionRules_map,
	&__CGameService3Service_getServerUpdateInfo_map,
	NULL
};

template <typename TClient>
inline HRESULT CGameService3ServiceT<TClient>::getRegisteredServerShortInfos(
		serverShortInfo** getRegisteredServerShortInfosResponse, int* __getRegisteredServerShortInfosResponse_nSizeIs
	)
{
    if ( getRegisteredServerShortInfosResponse == NULL )
		return E_POINTER;
    if( __getRegisteredServerShortInfosResponse_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CGameService3Service_getRegisteredServerShortInfos_struct __params;
	memset(&__params, 0x00, sizeof(__params));

	__atlsoap_hr = SetClientStruct(&__params, 0);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*getRegisteredServerShortInfosResponse = __params.getRegisteredServerShortInfosResponse;
	*__getRegisteredServerShortInfosResponse_nSizeIs = __params.__getRegisteredServerShortInfosResponse_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CGameService3ServiceT<TClient>::getServerAddressesByGameName(
		BSTR gameName, 
		serverAddress** getServerAddressesByGameNameResponse, int* __getServerAddressesByGameNameResponse_nSizeIs
	)
{
    if ( getServerAddressesByGameNameResponse == NULL )
		return E_POINTER;
    if( __getServerAddressesByGameNameResponse_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CGameService3Service_getServerAddressesByGameName_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.gameName = gameName;

	__atlsoap_hr = SetClientStruct(&__params, 1);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*getServerAddressesByGameNameResponse = __params.getServerAddressesByGameNameResponse;
	*__getServerAddressesByGameNameResponse_nSizeIs = __params.__getServerAddressesByGameNameResponse_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CGameService3ServiceT<TClient>::getServerRules(
		__int64 id, 
		rule** getServerRulesResponse, int* __getServerRulesResponse_nSizeIs
	)
{
    if ( getServerRulesResponse == NULL )
		return E_POINTER;
    if( __getServerRulesResponse_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CGameService3Service_getServerRules_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.id = id;

	__atlsoap_hr = SetClientStruct(&__params, 2);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*getServerRulesResponse = __params.getServerRulesResponse;
	*__getServerRulesResponse_nSizeIs = __params.__getServerRulesResponse_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CGameService3ServiceT<TClient>::getServersRoutines(
		serversRoutine** getServersRoutinesResponse, int* __getServersRoutinesResponse_nSizeIs
	)
{
    if ( getServersRoutinesResponse == NULL )
		return E_POINTER;
    if( __getServersRoutinesResponse_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CGameService3Service_getServersRoutines_struct __params;
	memset(&__params, 0x00, sizeof(__params));

	__atlsoap_hr = SetClientStruct(&__params, 3);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*getServersRoutinesResponse = __params.getServersRoutinesResponse;
	*__getServersRoutinesResponse_nSizeIs = __params.__getServersRoutinesResponse_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CGameService3ServiceT<TClient>::getSupportedGames(
		BSTR gameName, 
		game** getSupportedGamesResponse, int* __getSupportedGamesResponse_nSizeIs
	)
{
    if ( getSupportedGamesResponse == NULL )
		return E_POINTER;
    if( __getSupportedGamesResponse_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CGameService3Service_getSupportedGames_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.gameName = gameName;

	__atlsoap_hr = SetClientStruct(&__params, 4);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*getSupportedGamesResponse = __params.getSupportedGamesResponse;
	*__getSupportedGamesResponse_nSizeIs = __params.__getSupportedGamesResponse_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CGameService3ServiceT<TClient>::getSessionRules(
		__int64 id, 
		int sessionId, 
		rule** getSessionRulesResponse, int* __getSessionRulesResponse_nSizeIs
	)
{
    if ( getSessionRulesResponse == NULL )
		return E_POINTER;
    if( __getSessionRulesResponse_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CGameService3Service_getSessionRules_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.id = id;
	__params.sessionId = sessionId;

	__atlsoap_hr = SetClientStruct(&__params, 5);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*getSessionRulesResponse = __params.getSessionRulesResponse;
	*__getSessionRulesResponse_nSizeIs = __params.__getSessionRulesResponse_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CGameService3ServiceT<TClient>::getServerUpdateInfo(
		__int64 id, 
		int sessionId, 
		serverUpdateInfo* getServerUpdateInfoResponse
	)
{
    if ( getServerUpdateInfoResponse == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CGameService3Service_getServerUpdateInfo_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.id = id;
	__params.sessionId = sessionId;

	__atlsoap_hr = SetClientStruct(&__params, 6);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*getServerUpdateInfoResponse = __params.getServerUpdateInfoResponse;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CGameService3ServiceT<TClient>::GetFunctionMap()
{
	return __CGameService3Service_funcs;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CGameService3ServiceT<TClient>::GetHeaderMap()
{
	static const _soapmapentry __CGameService3Service_getRegisteredServerShortInfos_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CGameService3Service_getRegisteredServerShortInfos_atlsoapheader_map = 
	{
		0x2C2FB2D4,
		"getRegisteredServerShortInfos",
		L"getRegisteredServerShortInfos",
		sizeof("getRegisteredServerShortInfos")-1,
		sizeof("getRegisteredServerShortInfos")-1,
		SOAPMAP_HEADER,
		__CGameService3Service_getRegisteredServerShortInfos_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0xAF490AE8,
		"http://games.ws.services.mtu.ru",
		L"http://games.ws.services.mtu.ru",
		sizeof("http://games.ws.services.mtu.ru")-1
	};

	static const _soapmapentry __CGameService3Service_getServerAddressesByGameName_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CGameService3Service_getServerAddressesByGameName_atlsoapheader_map = 
	{
		0x5A91F26B,
		"getServerAddressesByGameName",
		L"getServerAddressesByGameName",
		sizeof("getServerAddressesByGameName")-1,
		sizeof("getServerAddressesByGameName")-1,
		SOAPMAP_HEADER,
		__CGameService3Service_getServerAddressesByGameName_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0xAF490AE8,
		"http://games.ws.services.mtu.ru",
		L"http://games.ws.services.mtu.ru",
		sizeof("http://games.ws.services.mtu.ru")-1
	};

	static const _soapmapentry __CGameService3Service_getServerRules_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CGameService3Service_getServerRules_atlsoapheader_map = 
	{
		0x5F4CA242,
		"getServerRules",
		L"getServerRules",
		sizeof("getServerRules")-1,
		sizeof("getServerRules")-1,
		SOAPMAP_HEADER,
		__CGameService3Service_getServerRules_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0xAF490AE8,
		"http://games.ws.services.mtu.ru",
		L"http://games.ws.services.mtu.ru",
		sizeof("http://games.ws.services.mtu.ru")-1
	};

	static const _soapmapentry __CGameService3Service_getServersRoutines_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CGameService3Service_getServersRoutines_atlsoapheader_map = 
	{
		0x611137A3,
		"getServersRoutines",
		L"getServersRoutines",
		sizeof("getServersRoutines")-1,
		sizeof("getServersRoutines")-1,
		SOAPMAP_HEADER,
		__CGameService3Service_getServersRoutines_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0xAF490AE8,
		"http://games.ws.services.mtu.ru",
		L"http://games.ws.services.mtu.ru",
		sizeof("http://games.ws.services.mtu.ru")-1
	};

	static const _soapmapentry __CGameService3Service_getSupportedGames_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CGameService3Service_getSupportedGames_atlsoapheader_map = 
	{
		0x1D573013,
		"getSupportedGames",
		L"getSupportedGames",
		sizeof("getSupportedGames")-1,
		sizeof("getSupportedGames")-1,
		SOAPMAP_HEADER,
		__CGameService3Service_getSupportedGames_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0xAF490AE8,
		"http://games.ws.services.mtu.ru",
		L"http://games.ws.services.mtu.ru",
		sizeof("http://games.ws.services.mtu.ru")-1
	};

	static const _soapmapentry __CGameService3Service_getSessionRules_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CGameService3Service_getSessionRules_atlsoapheader_map = 
	{
		0xF86AE9AF,
		"getSessionRules",
		L"getSessionRules",
		sizeof("getSessionRules")-1,
		sizeof("getSessionRules")-1,
		SOAPMAP_HEADER,
		__CGameService3Service_getSessionRules_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0xAF490AE8,
		"http://games.ws.services.mtu.ru",
		L"http://games.ws.services.mtu.ru",
		sizeof("http://games.ws.services.mtu.ru")-1
	};

	static const _soapmapentry __CGameService3Service_getServerUpdateInfo_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CGameService3Service_getServerUpdateInfo_atlsoapheader_map = 
	{
		0x85D2FC86,
		"getServerUpdateInfo",
		L"getServerUpdateInfo",
		sizeof("getServerUpdateInfo")-1,
		sizeof("getServerUpdateInfo")-1,
		SOAPMAP_HEADER,
		__CGameService3Service_getServerUpdateInfo_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0xAF490AE8,
		"http://games.ws.services.mtu.ru",
		L"http://games.ws.services.mtu.ru",
		sizeof("http://games.ws.services.mtu.ru")-1
	};


	static const _soapmap * __CGameService3Service_headers[] =
	{
		&__CGameService3Service_getRegisteredServerShortInfos_atlsoapheader_map,
		&__CGameService3Service_getServerAddressesByGameName_atlsoapheader_map,
		&__CGameService3Service_getServerRules_atlsoapheader_map,
		&__CGameService3Service_getServersRoutines_atlsoapheader_map,
		&__CGameService3Service_getSupportedGames_atlsoapheader_map,
		&__CGameService3Service_getSessionRules_atlsoapheader_map,
		&__CGameService3Service_getServerUpdateInfo_atlsoapheader_map,
		NULL
	};
	
	return __CGameService3Service_headers;
}

template <typename TClient>
ATL_NOINLINE inline void * CGameService3ServiceT<TClient>::GetHeaderValue()
{
	return this;
}

template <typename TClient>
ATL_NOINLINE inline const wchar_t * CGameService3ServiceT<TClient>::GetNamespaceUri()
{
	return L"http://games.ws.services.mtu.ru";
}

template <typename TClient>
ATL_NOINLINE inline const char * CGameService3ServiceT<TClient>::GetServiceName()
{
	return NULL;
}

template <typename TClient>
ATL_NOINLINE inline const char * CGameService3ServiceT<TClient>::GetNamespaceUriA()
{
	return "http://games.ws.services.mtu.ru";
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CGameService3ServiceT<TClient>::CallFunction(
	void *, 
	const wchar_t *, int,
	size_t)
{
	return E_NOTIMPL;
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CGameService3ServiceT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
{
	if (ppReader == NULL)
	{
		return E_INVALIDARG;
	}
	
	CComPtr<ISAXXMLReader> spReader = GetReader();
	if (spReader.p != NULL)
	{
		*ppReader = spReader.Detach();
		return S_OK;
	}
	return TClient::GetClientReader(ppReader);
}

} // namespace GameService3Service

template<>
inline HRESULT AtlCleanupValue<GameService3Service::protocol>(GameService3Service::protocol *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->className);
	AtlCleanupValue(&pVal->id);
	AtlCleanupValue(&pVal->name);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<GameService3Service::protocol>(GameService3Service::protocol *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->className, pMemMgr);
	AtlCleanupValueEx(&pVal->id, pMemMgr);
	AtlCleanupValueEx(&pVal->name, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<GameService3Service::game>(GameService3Service::game *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->annotation);
	AtlCleanupValue(&pVal->author);
	AtlCleanupValue(&pVal->commandLine);
	AtlCleanupValue(&pVal->direction);
	AtlCleanupValue(&pVal->files);
	AtlCleanupValue(&pVal->game_picture);
	AtlCleanupValue(&pVal->game_picture_name);
	AtlCleanupValue(&pVal->game_url);
	AtlCleanupValue(&pVal->id);
	AtlCleanupValue(&pVal->name);
	AtlCleanupValue(&pVal->playerPingPresent);
	AtlCleanupValue(&pVal->protocol0);
	AtlCleanupValue(&pVal->publication_date);
	AtlCleanupValue(&pVal->publisher);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<GameService3Service::game>(GameService3Service::game *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->annotation, pMemMgr);
	AtlCleanupValueEx(&pVal->author, pMemMgr);
	AtlCleanupValueEx(&pVal->commandLine, pMemMgr);
	AtlCleanupValueEx(&pVal->direction, pMemMgr);
	AtlCleanupValueEx(&pVal->files, pMemMgr);
	AtlCleanupValueEx(&pVal->game_picture, pMemMgr);
	AtlCleanupValueEx(&pVal->game_picture_name, pMemMgr);
	AtlCleanupValueEx(&pVal->game_url, pMemMgr);
	AtlCleanupValueEx(&pVal->id, pMemMgr);
	AtlCleanupValueEx(&pVal->name, pMemMgr);
	AtlCleanupValueEx(&pVal->playerPingPresent, pMemMgr);
	AtlCleanupValueEx(&pVal->protocol0, pMemMgr);
	AtlCleanupValueEx(&pVal->publication_date, pMemMgr);
	AtlCleanupValueEx(&pVal->publisher, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<GameService3Service::serverAddress>(GameService3Service::serverAddress *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->host);
	AtlCleanupValue(&pVal->port);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<GameService3Service::serverAddress>(GameService3Service::serverAddress *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->host, pMemMgr);
	AtlCleanupValueEx(&pVal->port, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<GameService3Service::rule>(GameService3Service::rule *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->key);
	AtlCleanupValue(&pVal->value);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<GameService3Service::rule>(GameService3Service::rule *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->key, pMemMgr);
	AtlCleanupValueEx(&pVal->value, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<GameService3Service::playerShortInfo>(GameService3Service::playerShortInfo *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->fullName);
	AtlCleanupValue(&pVal->ping);
	AtlCleanupValue(&pVal->score);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<GameService3Service::playerShortInfo>(GameService3Service::playerShortInfo *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->fullName, pMemMgr);
	AtlCleanupValueEx(&pVal->ping, pMemMgr);
	AtlCleanupValueEx(&pVal->score, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<GameService3Service::serverShortInfo>(GameService3Service::serverShortInfo *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->active);
	AtlCleanupValue(&pVal->currentPlayer);
	AtlCleanupValue(&pVal->gameID);
	AtlCleanupValue(&pVal->host);
	AtlCleanupValue(&pVal->id);
	AtlCleanupValue(&pVal->map);
	AtlCleanupValue(&pVal->maxPlayer);
	AtlCleanupValue(&pVal->name);
	AtlCleanupValue(&pVal->port);
	AtlCleanupValue(&pVal->regime);
	AtlCleanupValue(&pVal->sessionId);
	AtlCleanupValue(&pVal->sessionName);
	AtlCleanupValue(&pVal->status);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<GameService3Service::serverShortInfo>(GameService3Service::serverShortInfo *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->active, pMemMgr);
	AtlCleanupValueEx(&pVal->currentPlayer, pMemMgr);
	AtlCleanupValueEx(&pVal->gameID, pMemMgr);
	AtlCleanupValueEx(&pVal->host, pMemMgr);
	AtlCleanupValueEx(&pVal->id, pMemMgr);
	AtlCleanupValueEx(&pVal->map, pMemMgr);
	AtlCleanupValueEx(&pVal->maxPlayer, pMemMgr);
	AtlCleanupValueEx(&pVal->name, pMemMgr);
	AtlCleanupValueEx(&pVal->port, pMemMgr);
	AtlCleanupValueEx(&pVal->regime, pMemMgr);
	AtlCleanupValueEx(&pVal->sessionId, pMemMgr);
	AtlCleanupValueEx(&pVal->sessionName, pMemMgr);
	AtlCleanupValueEx(&pVal->status, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<GameService3Service::serversRoutine>(GameService3Service::serversRoutine *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->id);
	AtlCleanupValue(&pVal->name);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<GameService3Service::serversRoutine>(GameService3Service::serversRoutine *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->id, pMemMgr);
	AtlCleanupValueEx(&pVal->name, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<GameService3Service::serverUpdateInfo>(GameService3Service::serverUpdateInfo *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->active);
	AtlCleanupValue(&pVal->changeStatus);
	AtlCleanupValue(&pVal->id);
	AtlCleanupValue(&pVal->map);
	AtlCleanupValue(&pVal->maxPlayer);
	AtlCleanupArray(pVal->players, pVal->__players_nSizeIs);
	AtlCleanupValue(&pVal->sessionId);
	AtlCleanupValue(&pVal->sessionName);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<GameService3Service::serverUpdateInfo>(GameService3Service::serverUpdateInfo *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->active, pMemMgr);
	AtlCleanupValueEx(&pVal->changeStatus, pMemMgr);
	AtlCleanupValueEx(&pVal->id, pMemMgr);
	AtlCleanupValueEx(&pVal->map, pMemMgr);
	AtlCleanupValueEx(&pVal->maxPlayer, pMemMgr);
	if (pVal->players != NULL)
	{
		AtlCleanupArrayEx(pVal->players, pVal->__players_nSizeIs, pMemMgr);
		pMemMgr->Free(pVal->players);
	}
	AtlCleanupValueEx(&pVal->sessionId, pMemMgr);
	AtlCleanupValueEx(&pVal->sessionName, pMemMgr);
	return S_OK;
}
