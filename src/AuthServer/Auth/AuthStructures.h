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

#ifndef _PristonTale_Socket_h_
#define _PristonTale_Socket_h_
#include "Common/SharedDefines.h"
#endif /* _PristonTale_Socket_h_ */

/*
# DWORD = uint32
# WORD  = uint16
# BYTE  = uint8
# UINT  = uint32
*/

///< ENUMS
enum AccountLogin
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

struct PacketLoginUser : PacketStruct
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
};

struct PacketAccountLoginCode : PacketStruct
{
    uint32			                        sReserved;                      ///< NAME RESERVED          
    AccountLogin	                        sCode;                          ///< AccountLogin ENUM
    int32				                    sFailCode;                      ///< FAIL CODE
    char			                        sMessage[256];                  ///< REASON WHY 
};

struct PacketSending
{
    union
    {
        bool	                            sInUse;                         ///< NOT USED       
        uint32	                            sCount;                         ///< NOT USED
    };
    uint32		                            sSize;                          ///< PACKET SIZE?
    uint8		                            sPacket[8192];                  ///< PACKET
};