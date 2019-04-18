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

#ifndef _PristonTale_WorldStructures_h_
#define _PristonTale_WorldStructures_h_
#include "Common/SharedDefines.h"
#endif /* _PristonTale_WorldStructures_h_ */

/*
# DWORD = uint32
# WORD  = uint16
# BYTE  = uint8
# UINT  = uint32
*/

///< ENUMS
enum AccountLogin
{
    ACCOUNTLOGIN_Success = 1,	                        ///< SUCCESSFULL
    ACCOUNTLOGIN_ConnectionFailed = 0,	                        ///< CONNECTION FAILED
    ACCOUNTLOGIN_IncorrectName = -1,	                        ///< ACCOUNT DOES NOT EXIST
    ACCOUNTLOGIN_IncorrectPassword = -2,	                        ///< INCORRECT PASSWORD
    ACCOUNTLOGIN_Banned = -3,	                        ///< ACCOUNT IS BANNED
    ACCOUNTLOGIN_LoggedIn = -4,	                        ///< ACCOUNT ALREADY LOGGED IN
    ACCOUNTLOGIN_IPBanned = -5,	                        ///< ACCOUNT IP BANNED
    ACCOUNTLOGIN_YouAreBanned = -6,	                        ///< USER IP BANNED
    ACCOUNTLOGIN_TryAgain = -7,	                        ///< FAILED... RECONNECTING
    ACCOUNTLOGIN_Maintenance = -8,	                        ///< SERVER DOWN FOR MAINTENANCE
    ACCOUNTLOGIN_AccountNotActive = -16,	                        ///< ACCOUNT NOT ACTIVE
    ACCOUNTLOGIN_WrongVersion = -17,	                        ///< VERSION MISMATCH
    ACCOUNTLOGIN_TempBanned = -18,	                        ///< TEMPORARILY BANNED
};

enum CheckSum
{
    CHECK_SUM_PACKET = 0x6A
};

///< STRUCTURES
struct PacketReceiving
{
    PacketReceiving() : sInUse(0), sDelete(0) {}
    bool		  sInUse;                                                   ///< USE PACKET
    bool		  sDelete;                                                  ///< DELETE PACKET?
    uint8		  sPacket[8192];                                            ///< PACKET
};

typedef struct PacketStruct
{
    PacketStruct() : sLength(0), sEncKeyIndex(0), sEncrypted(0), sHeader(0) {}
    uint16	                                sLength;				        ///< PACKET LENGTH
    uint8	                                sEncKeyIndex;		            ///< PACKET ENCRYPTION KEY INDEX
    uint8	                                sEncrypted;		                ///< PACKET ENCRYPTED
    uint32	                                sHeader;				        ///< PACKET HEADER
}Packet;

typedef struct PacketLoginStruct : PacketStruct
{
    uint32									sUnk[3];                        ///< UNKNOWN    
    char									sUserID[32];                    ///< USERID
    char									sPassword[32];                  ///< PASSWORD
    char									sMacAddr[20];                   ///< MAC ADDRESS
    char									sPCName[32];                    ///< PC NAME
    uint32									sSerialHD;                      ///< SERIAL HARDWARE ID
    char									sVideoName[256];                ///< GAME NAME
    uint32									sWidthScreen;                   ///< GAME WIDTH
    uint32									sHeightScreen;                  ///< GAME HEIGHT
    uint32									sSystemOS;                      ///< SYSTEM OPERATING SYSTEM
}PacketLoginUser;

typedef struct PacketAccountLoginCodeStruct : PacketStruct
{
    uint32			                        sReserved;                      ///< NAME RESERVED          
    AccountLogin	                        sCode;                          ///< AccountLogin ENUM
    int32				                    sFailCode;                      ///< FAIL CODE
    char			                        sMessage[256];                  ///< REASON WHY 
}PacketAccountLoginCode;

typedef struct PacketSendingStruct
{
    union
    {
        bool	                            sInUse;                         ///< NOT USED       
        uint32	                            sCount;                         ///< NOT USED
    };
    uint32		                            sSize;                          ///< PACKET SIZE?
    uint8		                            sPacket[8192];                  ///< PACKET
}PacketSending;

typedef struct ChecksumFunctionStruct
{
    uint32								    sSize;                          ///< SIZE
    uint32								    sAddress;                       ///< ADDRRESS?
    uint32									sCheckSum;                      ///< UNKNOWN
}ChecksumFunction;

typedef struct PacketChecksumFunctionListStructs : Packet
{
    uint8									sKey;                           ///< KEY ENCRYPTION?
    ChecksumFunction					    sCheckSum[400];                 ///< UNKNOWN
}PacketChecksumFunctionList;

typedef struct PacketWindowListStruct : Packet
{
    int									    sCount;                         ///< NOT USED
    char									sWindowName[50][64];            ///< CLIENT WINDOW NAME 
}PacketWindowList;

typedef struct PacketPingStruct : Packet
{
    uint32	                                sTime;                          ///< TIME SINCE TICK
    uint32	                                sTick;                          ///< GetTickCount
}PacketPing;

typedef struct	TransCharInfoStruct
{
    char	                                sName[32];				        ///< CHARACTER NAME
    char	                                sModelName[64];		            ///< MODEL NAME
    char	                                sModelName2[64];		        ///< MODEL NAME 2
    uint32	                                sJobCode;			            ///< JOB CODE
    uint32		                            sLevel;					        ///< CHARACTER LEVEL
    uint32	                                sBrood;					        ///< BROOD
    uint32	                                sArmorCode;			            ///< ARMOR CODE
    uint32		                            sStartResult;			        /// BYTE Result?
    uint32		                            sPosX;                          ///< CHARACTER X LOCATION
    uint32		                            sPosZ;			                ///< CHARACTER Z LOCATION
    uint32	                                sTemp[13];		                ///< UNKNOWN
}TransCharInfo;

typedef struct PacketVersionStruct : Packet
{
    bool									sServerFull;                    ///< IS SERVER FULL? 
    uint32								    sVersion;                       ///< VERSION EXPECTED
    uint32								    sUnk2;                          ///< UNKNOWN - NOT USED?
}PacketVersion;

typedef struct	PacketUserInfoStruct : Packet
{
    char									sUserID[32];                    ///< ACCOUNT NAME
    uint32								    sCharCount;                     ///< NUMBER OF CHARACTERS
    TransCharInfo						    sCharacterData[6];              ///< CHARACTER DATA
}PacketUserInfo;

typedef struct PacketServerListStruct : Packet
{
    struct Header
    {
        char				                sServerName[16];                ///< SERVER NAME
        uint32				                sTime;                          ///< TIME IN UNIX
        uint32					            sTicket;                        ///< TICKET ID - NOT SURE WHAT THIS IS
        uint32				                sUnknown;                       ///< UNKNOWN    
        uint32					            sClanServerIndex;               ///< CLAN SERVER - NOT SURE WHAT THIS IS
        uint32					            sGameServers;                   ///< NUMBER OF SERVERS
    };

    struct Server
    {
        char				                sName[32];                      ///< NAME OF REALM
        char				                sIP[3][20];                     ///< IP OF REALM
        uint32					            sPort[4];                       ///< PORT OF REALM
    };

    Header					                sHeaderStruct;                  ///< HEADER STRUCT
    Server					                sServersStruct[4];              ///< SERVER STRUCT
}PacketServerList;