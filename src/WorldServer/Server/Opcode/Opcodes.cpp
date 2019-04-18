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
//-----------------------------------------------//
#include "WorldSocket.h"
//-----------------------------------------------//
namespace Priston
{
    //-----------------------------------------------//
    Opcodes* Opcodes::instance()
    {
        static Opcodes instance;
        return &instance;
    }
    //-----------------------------------------------//
    OpcodeHandler const Opcodes::emptyHandler =
    {
        "<none>",
        &WorldSocket::HandleNULL
    };
    //-----------------------------------------------//
    void Opcodes::InitializePackets()
    {
        ///< CMSG
        StoreClientPacket(ClientPacketHeader::CMSG_PING,               "CMSG_LOGIN_USER",                      &WorldSocket::HandlePing            );  

        ///< SMSG
        StoreServerPacket(ServerPacketHeader::SMSG_PONG,               "SMSG_PONG",                            &WorldSocket::HandleServerMessage   );

        LOG_INFO << "Loaded " << mClientOpcode.size() << " CMSG opcodes";
        LOG_INFO << "Loaded " << mServerOpcode.size() << " SMSG opcodes";
    }
    //-----------------------------------------------//
    void Opcodes::StoreClientPacket(const uint64& opcode, char const * name, void(WorldSocket::* handler)(const Packet* packet))
    {
        OpcodeHandler& ref = mClientOpcode[opcode];
        ref.name = name;
        ref.handler = handler;
    }
    //-----------------------------------------------//
    void Opcodes::StoreServerPacket(const uint64& opcode, char const * name, void(WorldSocket::* handler)(const Packet* packet))
    {
        OpcodeHandler& ref = mServerOpcode[opcode];
        ref.name = name;
        ref.handler = handler;
    }
    //-----------------------------------------------//
    OpcodeHandler const& Opcodes::GetClientPacket(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mClientOpcode.find(Id);
        if (itr != mClientOpcode.end())
            return itr->second;
        return emptyHandler;
    }
    //-----------------------------------------------//
    const char * Opcodes::GetClientPacketName(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mClientOpcode.find(Id);
        if (itr != mClientOpcode.end())
            return itr->second.name;
        return "NULL";
    }
    //-----------------------------------------------//
    OpcodeHandler const& Opcodes::GetServerPacket(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mClientOpcode.find(Id);
        if (itr != mServerOpcode.end())
            return itr->second;
        return emptyHandler;
    }
    //-----------------------------------------------//
    const char * Opcodes::GetServerPacketName(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mClientOpcode.find(Id);
        if (itr != mServerOpcode.end())
            return itr->second.name;
        return "NULL";
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//