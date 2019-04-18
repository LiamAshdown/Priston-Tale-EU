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

#ifndef _PristonTale_Opcodes_h_
#define _PristonTale_Opcodes_h_
#include "Common/SharedDefines.h"
#include "WorldStructures.h"

enum ClientPacketHeader
{
    CMSG_PING                                       = 0x435A0007,
    CMSG_CREATE_CHARACTER                           = 0x48470088,
};

enum ServerPacketHeader
{
    SMSG_VERSION                                    = 0x4847008A,
    SMSG_PONG                                       = 0x435A0007,
};

namespace Priston
{
    class WorldSocket;

    struct OpcodeHandler
    {
        char const* name;
        void (WorldSocket::*handler)(const Packet* packet);
    };

    typedef std::map<uint64, OpcodeHandler> OpcodeMap;

    class Opcodes
    {
    public:
        static Opcodes* instance();

    public:
        Opcodes() {}
        ~Opcodes() {}

    public:
        void InitializePackets();

        OpcodeHandler const& GetClientPacket(const uint64& Id);
        OpcodeHandler const& GetServerPacket(const uint64& Id);

        const char* GetClientPacketName(const uint64& Id);
        const char* GetServerPacketName(const uint64& Id);

    private:
        void StoreClientPacket(const uint64& opcode, char const* name, void (WorldSocket::*handler)(const Packet* packet));
        void StoreServerPacket(const uint64& opcode, char const* name, void (WorldSocket::*handler)(const Packet* packet));

    private:
        static OpcodeHandler const emptyHandler;
        OpcodeMap mClientOpcode;
        OpcodeMap mServerOpcode;
    };
}

#define sOpcode Priston::Opcodes::instance()

#endif /* _PristonTale_Opcodes_h_ */