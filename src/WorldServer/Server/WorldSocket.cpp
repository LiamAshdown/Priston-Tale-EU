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

#include "WorldSocket.h"
#include "Database/QueryDatabase.h"
#include "Config/Config.h"
#include "Network/Listener.h"

namespace SteerStone
{
    
    WorldSocket::WorldSocket(boost::asio::io_service& service, std::function<void(Socket*)> closeHandler) :
        Socket(service, std::move(closeHandler))
    {
    }
    
    WorldSocket::~WorldSocket()
    {
        IF_LOG(plog::debug)
            LOG_DEBUG << "Destructor WorldSocket called!";
    }
    
    bool WorldSocket::ProcessIncomingData()
    {
        if (const Packet* packet = DecryptPacket())
        {
            LOG_INFO << "Recieved Packet " << sOpcode->GetClientPacketName(packet->sHeader) << " " << packet->sHeader;

            switch (packet->sHeader)
            {
            case CMSG_PING:
                HandlePing(packet);
                break;
            default:
                break;
            }

            return true;
        }

        return false;
    }
    
    void WorldSocket::SendPacket(const uint8* packet, const uint16& length)
    {
        // TODO; Optimize this code. We don't need to create a new struct.
        //       We also need to add a LOG_INFO << Sent packet...
        PacketSending packetSending;
        packetSending.sSize = length;

        memcpy(packetSending.sPacket, packet, length);

        Write((const char*)&packetSending.sPacket, packetSending.sSize);
    }
    
    const Packet* WorldSocket::DecryptPacket()
    {
        PacketReceiving packetRecieving{};
        if (!Read((char*)packetRecieving.sPacket, ReadLengthRemaining()))
            return nullptr;

        return (Packet*)&packetRecieving.sPacket;
    }
    
    void WorldSocket::SendVersionCheck()
    {
        // Send expected version to client
        PacketVersion packetVersion;
        packetVersion.sLength = sizeof(PacketVersion);
        packetVersion.sHeader = ServerPacketHeader::SMSG_VERSION;
        packetVersion.sServerFull = Priston::GlobalConnections::instance()->CurrentConnections >= sConfig->GetIntDefault("MaximumConnections", 1000) ? true : false;
        packetVersion.sUnk2 = 0;
        packetVersion.sEncKeyIndex = 0;
        packetVersion.sEncrypted = 0;
        packetVersion.sVersion = sConfig->GetIntDefault("ClientVersion", 1048);
        SendPacket((uint8*)(Packet*)&packetVersion, packetVersion.sLength);
    }
    
    void WorldSocket::HandlePing(const Packet* packet)
    {
        if (((PacketPing*)&packet)->sLength != sizeof(PacketPing))
            return;

        ((PacketPing*)&packet)->sTick = GetTickCount();
        SendPacket((uint8*)&packet, packet->sLength);
    }
}
