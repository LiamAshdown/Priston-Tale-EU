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
#include "Common/SHA1.h"
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
        IF_LOG(plog::debug)
            LOG_DEBUG << "Destructor AuthSocket called!";
    }
    //-----------------------------------------------//
    bool AuthSocket::ProcessIncomingData()
    {
        if (Packet* packet = DecryptPacket())
        {
            LOG_INFO << "Recieved " << sOpcode->GetOpCodeName(packet->sHeader) << " " << packet->sHeader;

            ExecutePacket(sOpcode->GetPacket(packet->sHeader), packet);
            return true;
        }

        return false;
    }
    //-----------------------------------------------//
    void AuthSocket::SendPacket(const uint8* packet, const uint16& length)
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
        if (!Read((char*)packetRecieving.sPacket, ReadLengthRemaining()))
            return nullptr;

        return (Packet*)&packetRecieving.sPacket;
    }
    //-----------------------------------------------//
    void AuthSocket::ExecutePacket(const OpcodeHandler& opHandler, const Packet* packet)
    {
        (this->*opHandler.handler)(packet);
    }
    //-----------------------------------------------//
    void AuthSocket::HandleNULL(const Packet* packet)
    {
        LOG_ERROR << sOpcode->GetOpCodeName(packet->sHeader) << " not currently handled!";
    }
    //-----------------------------------------------//
    void AuthSocket::HandleServerMessage(const Packet* packet)
    {
    }
    //-----------------------------------------------//
                ////////////////////////
                //      HANDLERS      //
                ///////////////////////
    //-----------------------------------------------//
    void AuthSocket::HandleLoginUser(const Packet* packet)
    {
        // Convert our Packet into PacketLoginUser
        PacketLoginUser* packetUser = (PacketLoginUser*)packet;

        // Query Auth account database
        QueryDatabase database("auth");
        database.PrepareQuery("SELECT * FROM account WHERE username = ?");
        database.GetStatement()->setString(1, static_cast<std::string>(packetUser->sUserID).c_str());
        database.ExecuteQuery();

        // If username doesn't exist in database
        if (!database.GetResult())
        {
            PacketAccountLoginCode accountLogin;
            accountLogin.sLength = sizeof(accountLogin);
            accountLogin.sHeader = PacketsHeader::SMSG_ACCOUNT_LOGIN_CODE;
            accountLogin.sReserved = 0;
            accountLogin.sCode = AccountLogin::ACCOUNTLOGIN_IncorrectName;
            accountLogin.sFailCode = 1;
            accountLogin.sEncKeyIndex = 0;
            accountLogin.sEncrypted = 1;
            SendPacket((uint8*)(Packet*)&accountLogin, accountLogin.sLength);
            return;
        }

        Field* fields = database.Fetch();
        std::string hashedPassword = fields->GetString(3);

        // If our hashed password does not match the same in the database, then send incorrect password packet
        if (CalculateSHA1Hash(boost::to_upper_copy<std::string>(static_cast<std::string>(packetUser->sUserID)
            + ":" + static_cast<std::string>(packetUser->sPassword))) != hashedPassword)
        {
            PacketAccountLoginCode accountLogin;
            accountLogin.sLength = sizeof(accountLogin);
            accountLogin.sHeader = PacketsHeader::SMSG_ACCOUNT_LOGIN_CODE;
            accountLogin.sReserved = 0;
            accountLogin.sCode = AccountLogin::ACCOUNTLOGIN_IncorrectPassword;
            accountLogin.sFailCode = 1;
            accountLogin.sEncKeyIndex = 0;
            accountLogin.sEncrypted = 1;

            SendPacket((uint8*)(Packet*)&accountLogin, accountLogin.sLength);
            return;
        }

        PacketChecksumFunctionList packetCheck;
        packetCheck.sLength       = sizeof(PacketChecksumFunctionList);
        packetCheck.sHeader       = PacketsHeader::SMSG_CHECK_SUM;
        packetCheck.sKey          = CHECK_SUM_PACKET;
        packetCheck.sEncKeyIndex  = 0;
        packetCheck.sEncrypted    = 1;

        SendPacket((uint8*)(Packet*)&packetCheck, packetCheck.sLength);

        PacketWindowList packetWindow;
        packetWindow.sLength      = sizeof(PacketWindowList);
        packetWindow.sHeader      = PacketsHeader::SMSG_WINDOW_LIST;
        packetWindow.sEncKeyIndex = 0;
        packetWindow.sEncrypted   = 1;

        SendPacket((uint8*)(Packet*)&packetWindow, packetWindow.sLength);
    }
    //-----------------------------------------------//
    void AuthSocket::HandlePing(const Packet* packet)
    {
        if (((PacketPing*)packet)->sLength != sizeof(PacketPing))
            return;

        ((PacketPing*)packet)->sTick = GetTickCount();
        SendPacket((uint8*)packet, packet->sLength);
    }
    //-----------------------------------------------//
}