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

#ifndef _PristonTale_WorldSocket_h_
#define _PristonTale_WorldSocket_h_
#include "Network/Socket.h"
#include "Session.h"
#include "Opcodes.h"

namespace SteerStone
{
    class WorldSocket : public Socket
    {
    public:
        WorldSocket(boost::asio::io_service& service, std::function<void(Socket*)> closeHandler);
        ~WorldSocket();

    private:
        virtual bool ProcessIncomingData() override;
        virtual void SendVersionCheck() override;

    private:
        void HandlePing(const Packet* packet);
        void SendPacket(const uint8* packet, const uint16& length);
        const Packet* DecryptPacket();
    };
}

#endif /* _PristonTale_WorldSocket_h_ */