/*
* Priston Tale EU
* Copyright (C) 2019
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _PristonTale_AuthStructures_h_
#define _PristonTale_AuthStructures_h_
#include "Common/SharedDefines.h"
#endif /* _PristonTale_AuthStructures_h_ */

/*
# DWORD = uint32
# WORD  = uint16
# BYTE  = uint8
# UINT  = uint32
*/

///< ENUMS
enum LoginError
{
	ACCOUNTLOGIN_Success			        = 1,	                        ///< SUCCESSFULL
	ACCOUNTLOGIN_ConnectionFailed	        = 0,	                        ///< CONNECTION FAILED
	ACCOUNTLOGIN_IncorrectName		        = -1,	                        ///< ACCOUNT DOES NOT EXIST
	ACCOUNTLOGIN_IncorrectPassword	        = -2,	                        ///< INCORRECT PASSWORD
	ACCOUNTLOGIN_Banned				        = -3,	                        ///< ACCOUNT IS BANNED
	ACCOUNTLOGIN_LoggedIn			        = -4,	                        ///< ACCOUNT ALREADY LOGGED IN
	ACCOUNTLOGIN_IPBanned			        = -5,	                        ///< ACCOUNT IP BANNED
	ACCOUNTLOGIN_YouAreBanned		        = -6,	                        ///< USER IP BANNED
	ACCOUNTLOGIN_TryAgain			        = -7,	                        ///< FAILED... RECONNECTING
	ACCOUNTLOGIN_Maintenance		        = -8,	                        ///< SERVER DOWN FOR MAINTENANCE
	ACCOUNTLOGIN_AccountNotActive	        = -16,	                        ///< ACCOUNT NOT ACTIVE
	ACCOUNTLOGIN_WrongVersion		        = -17,	                        ///< VERSION MISMATCH
	ACCOUNTLOGIN_TempBanned			        = -18,	                        ///< TEMPORARILY BANNED
};

enum CheckSum
{
    CHECK_SUM_PACKET                        = 0x6A
};

///< STRUCTURES
struct PacketReceiving
{
    PacketReceiving() : InUse(0), Delete(0) {}
    bool		                            InUse;                      ///< USE PACKET
    bool		                            Delete;                     ///< DELETE PACKET?
    uint8		                            Packet[8192];               ///< PACKET
};

typedef struct PacketStruct
{
    PacketStruct() : Length(0), EncKeyIndex(0), Encrypted(0), PacketHeader(0) {}
    uint16	                                Length;				        ///< PACKET LENGTH
    uint8	                                EncKeyIndex;		        ///< PACKET ENCRYPTION KEY INDEX
    uint8	                                Encrypted;		            ///< PACKET ENCRYPTED
    uint32	                                PacketHeader;				///< PACKET HEADER
}Packet;

typedef struct PacketLoginStruct : PacketStruct
{
	uint32									Unk[3];                        ///< UNKNOWN    
	char									UserID[32];                    ///< USERID
	char									Password[32];                  ///< PASSWORD
	char									MacAddr[20];                   ///< MAC ADDRESS
	char									PCName[32];                    ///< PC NAME
    uint32									SerialHD;                      ///< SERIAL HARDWARE ID
	char									VideoName[256];                ///< GAME NAME
    uint32									WidthScreen;                   ///< GAME WIDTH
    uint32									HeightScreen;                  ///< GAME HEIGHT
    uint32									SystemOS;                      ///< SYSTEM OPERATING SYSTEM
}PacketLoginUser;

typedef struct PacketAccountLoginCodeStruct : PacketStruct
{
    uint32			                        Reserved;                      ///< NAME RESERVED          
    LoginError	                            Code;                          ///< LoginError ENUM
    int32				                    FailCode;                      ///< FAIL CODE
    char			                        Message[256];                  ///< REASON WHY 
}PacketAccountLoginCode;

typedef struct PacketSendingStruct
{
    union
    {
        bool	                            InUse;                         ///< NOT USED       
        uint32	                            Count;                         ///< NOT USED
    };
    uint32		                            Size;                          ///< PACKET SIZE?
    uint8		                            Packet[8192];                  ///< PACKET
}PacketSending;

typedef struct ChecksumFunctionStruct
{
    uint32								    Size;                          ///< SIZE
    uint32								    Address;                       ///< ADDRRESS?
    uint32									CheckSum;                      ///< UNKNOWN
}ChecksumFunction;

typedef struct PacketChecksumFunctionListStructs : Packet
{
    uint8									Key;                           ///< KEY ENCRYPTION?
    ChecksumFunction					    CheckSum[400];                 ///< UNKNOWN
}PacketChecksumFunctionList;

typedef struct PacketWindowListStruct : Packet
{
    int									    Count;                         ///< NOT USED
    char									WindowName[50][64];            ///< CLIENT WINDOW NAME 
}PacketWindowList;

typedef struct PacketPingStruct : Packet
{
    uint32	                                Time;                          ///< TIME SINCE TICK
    uint32	                                Tick;                          ///< GetTickCount
}PacketPing;

typedef struct	TransCharInfoStruct
{
    char	                                Name[32];				        ///< CHARACTER NAME
    char	                                ModelName[64];		            ///< MODEL NAME
    char	                                ModelName2[64];		        ///< MODEL NAME 2
    uint32	                                JobCode;			            ///< JOB CODE
    uint32		                            Level;					        ///< CHARACTER LEVEL
    uint32	                                Brood;					        ///< BROOD
    uint32	                                ArmorCode;			            ///< ARMOR CODE
    uint32		                            StartResult;			        /// BYTE Result?
    uint32		                            PosX;                          ///< CHARACTER X LOCATION
    uint32		                            PosZ;			                ///< CHARACTER Z LOCATION
    uint32	                                Temp[13];		                ///< UNKNOWN
}TransCharInfo;

typedef struct PacketVersionStruct : Packet
{
    bool									ServerFull;                    ///< IS SERVER FULL? 
    uint32								    Version;                       ///< VERSION EXPECTED
    uint32								    Unk2;                          ///< UNKNOWN - NOT USED?
}PacketVersion;

typedef struct	PacketUserInfoStruct : Packet
{
    char									UserID[32];                    ///< ACCOUNT NAME
    uint32								    CharCount;                     ///< NUMBER OF CHARACTERS
    TransCharInfo						    CharacterData[6];              ///< CHARACTER DATA
}PacketUserInfo;

typedef struct PacketServerListStruct : Packet
{
    struct Header
    {
        char				                ServerName[16];                ///< SERVER NAME
        uint32				                Time;                          ///< TIME IN UNIX
        uint32					            Ticket;                        ///< TICKET ID - NOT SURE WHAT THIS IS
        uint32				                Unknown;                       ///< UNKNOWN    
        uint32					            ClanServerIndex;               ///< CLAN SERVER - NOT SURE WHAT THIS IS
        uint32					            GameServers;                   ///< NUMBER OF SERVERS
    };

    struct Server
    {
        char				                Name[32];                      ///< NAME OF REALM
        char				                IP[3][20];                     ///< IP OF REALM
        uint32					            Port[4];                       ///< PORT OF REALM
    };

    Header					                HeaderStruct;                  ///< HEADER STRUCT
    Server					                ServersStruct[4];              ///< SERVER STRUCT
}PacketServerList;