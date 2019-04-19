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

#include "AuthSocket.h"
#include "Common/SHA1.h"
#include "Config/Config.h"
#include "Common/Timer.h"

namespace SteerStone
{
    /// Constructor 
    /// @p_Service : Boost Service
    /// @p_CloseHandler : Close Handler Custom function
    AuthSocket::AuthSocket(boost::asio::io_service& service, std::function<void(Socket*)> closeHandler) :
        Socket(service, std::move(closeHandler))
    {
        p_PingDiff = sTimer->GetServerTime();
    }
    
    /// Deconstructor
    AuthSocket::~AuthSocket()
    {
    }
    
    /// ProcessIncomingData
    /// Handle incoming data from client
    bool AuthSocket::ProcessIncomingData()
    {
        if (const Packet* l_Packet = DecryptPacket())
        {
            LOG_INFO << "[INCOMING]: " << "[" << l_Packet->PacketHeader << "] [" << sOpcode->GetClientPacket(l_Packet->PacketHeader).name << "]";

            ExecutePacket(sOpcode->GetClientPacket(l_Packet->PacketHeader), l_Packet);
            return true;
        }
        return false;
    }

    /// SendVersionCheck
    /// Send expection version to client
    void AuthSocket::SendVersionCheck()
    {
        PacketVersion packetVersion;
        packetVersion.Length       = sizeof(PacketVersion);
        packetVersion.PacketHeader = ServerPacketHeader::SMSG_VERSION;
        packetVersion.ServerFull   = 0; ///< TODO; Check if server full by database
        packetVersion.Unk2         = 0;
        packetVersion.EncKeyIndex  = 0;
        packetVersion.Encrypted    = 0;
        packetVersion.Version      = sConfig->GetIntDefault("ClientVersion", 1048);
        SendPacket((uint8*)(Packet*)&packetVersion, packetVersion.Length);
    }
    
    /// SendPacket 
    /// @p_Packet : Buffer which holds our data to be send to the client
    /// @p_Length : Size of buffer
    void AuthSocket::SendPacket(uint8* p_Packet, const uint16& p_Length)
    {
        PacketSending l_Packet;
        l_Packet.Size = p_Length;

        memcpy(l_Packet.Packet, p_Packet, p_Length);

        Write((const char*)&l_Packet.Packet, l_Packet.Size);
    }
    
    /// DecryptPacket
    /// Decrypt incoming packet from client
    const Packet* AuthSocket::DecryptPacket()
    {
        PacketReceiving packetRecieving{};
        if (!Read((char*)packetRecieving.Packet, ReadLengthRemaining()))
            return nullptr;

        return (Packet*)&packetRecieving.Packet;
    }
   
    /// ExecutePacket
    /// @p_OpHandler : Function which will be called
    /// @p_Packet : Packet we are passing to the function
    void AuthSocket::ExecutePacket(const OpcodeHandler& p_OpHandler, const Packet* p_Packet)
    {
        (this->*p_OpHandler.handler)(p_Packet);
    }
     
    /// HandleServerMessage
    /// Default Server handler
    void AuthSocket::HandleServerMessage(const Packet* p_Packet)
    {
    }

    /// HandleNULl
    /// Default Handler if we have not handled the packet yet
    /// @p_Packet : Packet sent by client
    void AuthSocket::HandleNULL(const Packet* p_Packet)
    {
    }
} ///< NAMESPACE STEERSTONE
