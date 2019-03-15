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

///< STRUCTURES
struct PacketReceiving
{
    PacketReceiving() : sInUse(0), sDelete(0) {}
    bool		                            sInUse;                         ///< USE PACKET
    bool		                            sDelete;                        ///< DELETE PACKET?
    uint8		                            sPacket[8192];                  ///< PACKET
};

typedef struct PacketStruct
{
    PacketStruct() : sLength(0), sEncKeyIndex(0), sEncrypted(0), sHeader(0) {}
    uint16	                                sLength;				        ///< PACKET LENGTH
    uint8	                                sEncKeyIndex;		            ///< PACKET ENCRYPTION KEY INDEX
    uint8	                                sEncrypted;		                ///< PACKET ENCRYPTED
    uint32	                                sHeader;				        ///< PACKET HEADER
}Packet;

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
