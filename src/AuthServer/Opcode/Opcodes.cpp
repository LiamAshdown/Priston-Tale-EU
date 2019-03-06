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
#include "AuthSocket.h"
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
        &AuthSocket::HandleNULL
    };
    //-----------------------------------------------//
    void Opcodes::InitializePackets()
    {
        StorePacket(PacketsHeader::CMSG_LOGIN_USER,         "CMSG_LOGIN_USER",                      &AuthSocket::HandleLoginUser        );
        StorePacket(PacketsHeader::CMSG_SAVE_DATA,          "CMSG_SAVE_DATA",                       &AuthSocket::HandleNULL             );
        StorePacket(PacketsHeader::CMSG_SELECT_CHARACTER,   "CMSG_SELECT_CHARACTER",                &AuthSocket::HandleNULL             );
        StorePacket(PacketsHeader::CMSG_USER_INFO,          "CMSG_USER_INFO",                       &AuthSocket::HandleNULL             );
        StorePacket(PacketsHeader::CMSG_CREATE_CHARACTER,   "CMSG_CREATE_CHARACTER",                &AuthSocket::HandleNULL             );
        StorePacket(PacketsHeader::CMSG_DELETE_CHARACTER,   "CMSG_DELETE_CHARACTER",                &AuthSocket::HandleNULL             );
        StorePacket(PacketsHeader::CMSG_VERSION,            "CMSG_VERSION",                         &AuthSocket::HandleNULL             );
        StorePacket(PacketsHeader::CMSG_ACCOUNT_LOGIN_CODE, "CMSG_ACCOUNT_LOGIN_CODE",              &AuthSocket::HandleNULL             );
        StorePacket(PacketsHeader::CMSG_PING,               "CMSG_PING",                            &AuthSocket::HandleNULL             );
    }
    //-----------------------------------------------//
    void Opcodes::StorePacket(const uint64& opcode, char const * name, void(AuthSocket::* handler)(Packet packet))
    {
        OpcodeHandler& ref = mOpcode[opcode];
        ref.name = name;
        ref.handler = handler;
    }
    //-----------------------------------------------//
    OpcodeHandler const& Opcodes::GetPacket(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mOpcode.find(Id);
        if (itr != mOpcode.end())
            return itr->second;
        return emptyHandler;
    }
    //-----------------------------------------------//
    const char * Opcodes::GetOpCodeName(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mOpcode.find(Id);
        if (itr != mOpcode.end())
            return itr->second.name;
        return "NULL";
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//