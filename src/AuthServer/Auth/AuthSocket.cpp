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
#include "Database/QueryDatabase.h"
#include "Database/Fields.h"
//-----------------------------------------------//
namespace Priston
{
    //-----------------------------------------------//
    AuthSocket::AuthSocket(boost::asio::io_service& service, std::function<void(Socket*)> closeHandler) :
        Socket(service, std::move(closeHandler))
    {
    }
    //-----------------------------------------------//
    AuthSocket::~AuthSocket()
    {
    }
    //-----------------------------------------------//
    bool AuthSocket::ProcessIncomingData()
    {
        if (Packet* packet = DecryptPacket())
        {
            LOG_INFO << "Reading " << sOpcode->GetOpCodeName(packet->sHeader);

            ExecutePacket(sOpcode->GetPacket(packet->sHeader), *packet);
            return true;
        }

        return false;
    }
    //-----------------------------------------------//
    void AuthSocket::EncryptPacket(const uint8* packet, const uint16& length)
    {
        // TODO; Optimize this code. We don't need to create a new struct.
        //       We also need to add a LOG_INFO << Sent packet...
        PacketSending packetSending;
        packetSending.sSize = length;

        memcpy(packetSending.sPacket, packet, length);

        Write((const char*)&packetSending.sPacket, packetSending.sSize);
    }
    //-----------------------------------------------//
    Packet* AuthSocket::DecryptPacket()
    {
        PacketReceiving packetRecieving{};
        if (!Read((char*)&packetRecieving.sPacket, ReadLengthRemaining()))
            return nullptr;

        return (Packet*)packetRecieving.sPacket;
    }
    //-----------------------------------------------//
    void AuthSocket::ExecutePacket(const OpcodeHandler& opHandler, Packet packet)
    {
        (this->*opHandler.handler)(packet);
    }
    //-----------------------------------------------//
    void AuthSocket::HandleNULL(Packet packet)
    {
        LOG_ERROR << sOpcode->GetOpCodeName(packet.sHeader) << " not currently handled!";
    }
    //-----------------------------------------------//
                ////////////////////////
                //      HANDLERS      //
                ///////////////////////
    //-----------------------------------------------//
    void AuthSocket::HandleLoginUser(Packet packet)
    {
        // Convert our Packet into PacketLoginUser
        PacketLoginUser* packetUser = (PacketLoginUser*)&packet;

        // This is where we check if the account exists
        QueryAuthDatabase database;
        database.Query("select * from auth penis");

        Field fields = database.GetResult();

        fields[0].GetString(2);

        AccountLogin errorCode = ACCOUNTLOGIN_Success;

        PacketAccountLoginCode loginCode;
        loginCode.sLength = sizeof(loginCode);
        loginCode.sHeader = CMSG_ACCOUNT_LOGIN_CODE;
        loginCode.sReserved = 0;
        loginCode.sCode = ACCOUNTLOGIN_IncorrectName;
        loginCode.sFailCode = 1;
        loginCode.sEncKeyIndex = 0;
        loginCode.sEncrypted = 1;

        EncryptPacket((uint8*)(Packet*)&loginCode, loginCode.sLength);
    }
    //-----------------------------------------------//
}