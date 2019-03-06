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

#ifndef _PristonTale_AuthSocket_h_
#define _PristonTale_AuthSocket_h_
#include "Network/Socket.h"
#include "Opcodes.h"

namespace Priston
{
    class AuthSocket : public Socket
    {
    public:
        AuthSocket(boost::asio::io_service& service, std::function<void(Socket*)> closeHandler);
        ~AuthSocket();

    public:
        ////////////////////////
        //      HANDLERS      //
        ///////////////////////
        void ExecutePacket(const OpcodeHandler& opHandler, Packet packet);

        void HandleNULL(Packet packet);
        void HandleLoginUser(Packet packet);

    private:
        virtual bool ProcessIncomingData() override;

    private:
        void EncryptPacket(const uint8* packet, const uint16& length);
        Packet* DecryptPacket();
    };
}

#endif /* _PristonTale_AuthSocket_h_ */