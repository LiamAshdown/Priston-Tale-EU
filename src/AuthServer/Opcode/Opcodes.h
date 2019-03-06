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
#include "AuthStructures.h"

enum PacketsHeader
{
    CMSG_SERVER_LIST                                = 0x484700C0,
    CMSG_SAVE_DATA                                  = 0x48470081,
	CMSG_SELECT_CHARACTER							= 0x48470082,
	CMSG_USER_INFO									= 0x48470086,
	CMSG_CREATE_CHARACTER							= 0x48470088,
	CMSG_DELETE_CHARACTER							= 0x48470087,
	CMSG_VERSION									= 0x4847008A, // edit chaos
	CMSG_ACCOUNT_LOGIN_CODE							= 0x48470023,
	CMSG_LOGIN_USER								    = 0x48480001,
    CMSG_PING                                       = 0x435A0007
};

namespace Priston
{
    class AuthSocket;

    struct OpcodeHandler
    {
        char const* name;
        void (AuthSocket::*handler)(Packet packet);
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
        OpcodeHandler const& GetPacket(const uint64& Id);

        const char* GetOpCodeName(const uint64& Id);

    private:
        void StorePacket(const uint64& opcode, char const* name, void (AuthSocket::*handler)(Packet packet));

    private:
        static OpcodeHandler const emptyHandler;
        OpcodeMap mOpcode;
    };
}

#define sOpcode Priston::Opcodes::instance()

#endif /* _PristonTale_Opcodes_h_ */