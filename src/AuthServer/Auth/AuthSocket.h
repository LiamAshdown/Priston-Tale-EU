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

#ifndef AUTH_AUTH_SOCKET_h
#define AUTH_AUTH_SOCKET_h
#include "Opcodes.h"
#include "Network/Listener.h"
#include "Database/QueryDatabase.h"
#endif /* A UTH_AUTH_SOCKET_h */

namespace SteerStone
{
    /// Class which inherits from Socket class when a successfull connection is made
    class AuthSocket : public Socket
    {
    public:
        /// Constructor 
        /// @p_Service : Boost Service
        /// @p_CloseHandler : Close Handler Custom function
        AuthSocket(boost::asio::io_service& p_IoService, std::function<void(Socket*)> p_CloseHandler);

        /// Deconstructor
        ~AuthSocket();

    public:
        /// ExecutePacket
        /// @p_OpHandler : Function which will be called
        /// @p_Packet : Packet we are passing to the function
        void ExecutePacket(const OpcodeHandler& p_OpHandler, const Packet* p_Packet);

        /// HandleLoginUser
        /// Checks Login data and sends back result to client
        /// @p_Packet : Packet sent by client
        void HandleLoginUser(const Packet* p_Packet);

        /// HandlePing
        /// Keep connection alive by client and server
        /// @p_Packet : Packet sent by client
        void HandlePing(const Packet* p_Packet);

        /// HandleServerMessage
        /// Default Server handler
        void HandleServerMessage(const Packet* p_Packet);

        /// HandleNULl
        /// Default Handler if we have not handled the packet yet
        /// @p_Packet : Packet sent by client
        void HandleNULL(const Packet* p_Packet);

    private:
        /// ProcessIncomingData
        /// Handle incoming data from client
        virtual bool ProcessIncomingData() override;

        /// SendVersionCheck
        /// Send expection version to client
        virtual void SendVersionCheck() override;

    private:
        /// SendPacket 
        /// @p_Packet : Buffer which holds our data to be send to the client
        /// @p_Length : Size of buffer
        void SendPacket(uint8* p_Packet, const uint16& p_Length);

        /// DecryptPacket
        /// Decrypt incoming packet from client
        const Packet* DecryptPacket();

        /// SendUserSuccess
        /// Client can now login into the game server
        /// @p_Result : Database result
        void SendUserSuccess(Result* p_Result);

    private:
        uint32 p_PingDiff;          ///< Last time server recieved a pong from client
    };
} ///< NAMESPACE STEERSTONE