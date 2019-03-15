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
#include "Common/SHA1.h"
#include "Config/Config.h"
#include "Network/Listener.h"
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
        if (const Packet* packet = DecryptPacket())
        {
            LOG_INFO << "RECIEVED PACKET: " << packet->sHeader;

            ExecutePacket(sOpcode->GetClientPacket(packet->sHeader), packet);
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
    const Packet* AuthSocket::DecryptPacket()
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
        LOG_ERROR << sOpcode->GetClientPacketName(packet->sHeader) << " not currently handled!";
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
        database.PrepareQuery("SELECT id, user_name, sha_pass FROM account WHERE user_name = ?");
        database.GetStatement()->setString(1, static_cast<std::string>(packetUser->sUserID).c_str());
        database.ExecuteQuery();

        // If username doesn't exist in database
        if (!database.GetResult())
        {
            PacketAccountLoginCode accountLogin;
            accountLogin.sLength = sizeof(accountLogin);
            accountLogin.sHeader = ServerPacketHeader::SMSG_ACCOUNT_LOGIN_CODE;
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
            accountLogin.sHeader = ServerPacketHeader::SMSG_ACCOUNT_LOGIN_CODE;
            accountLogin.sReserved = 0;
            accountLogin.sCode = AccountLogin::ACCOUNTLOGIN_IncorrectPassword;
            accountLogin.sFailCode = 1;
            accountLogin.sEncKeyIndex = 0;
            accountLogin.sEncrypted = 1;

            SendPacket((uint8*)(Packet*)&accountLogin, accountLogin.sLength);
            return;
        }

        PacketChecksumFunctionList packetCheck;
        packetCheck.sLength = sizeof(PacketChecksumFunctionList);
        packetCheck.sHeader = ServerPacketHeader::SMSG_CHECK_SUM;
        packetCheck.sKey = CHECK_SUM_PACKET;
        packetCheck.sEncKeyIndex = 0;
        packetCheck.sEncrypted = 1;
        SendPacket((uint8*)(Packet*)&packetCheck, packetCheck.sLength);

        SendUserSuccess(fields);
    }
    //-----------------------------------------------//
    void AuthSocket::HandlePing(const Packet* packet)
    {
        if (((PacketPing*)&packet)->sLength != sizeof(PacketPing))
            return;

        ((PacketPing*)&packet)->sTick = GetTickCount();
        SendPacket((uint8*)&packet, packet->sLength);
    }
    //-----------------------------------------------//
    void AuthSocket::SendVersionCheck()
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
    //-----------------------------------------------//
    void AuthSocket::SendUserSuccess(Field* fields)
    {
        PacketUserInfo packetUser;
        packetUser.sLength = sizeof(PacketUserInfo);
        packetUser.sHeader = ServerPacketHeader::SMSG_USER_INFO;
        strcpy(packetUser.sUserID, fields->GetString(2).c_str());
        packetUser.sCharCount = 0; // TODO; code character data
        packetUser.sEncKeyIndex = 0;
        packetUser.sEncrypted = 1;
        SendPacket((uint8*)(Packet*)&packetUser, packetUser.sLength);

        QueryDatabase database("auth");
        database.PrepareQuery("SELECT id, ip_address, port, server_name, realm_name FROM server_realms");
        database.ExecuteQuery();

        if (!database.GetResult())
            return;

        // We already check whether we have a realm or not when we first boot up the server
        fields = database.Fetch();

        PacketServerList serverList;
        serverList.sLength = sizeof(Packet) + sizeof(PacketServerList::Header);
        serverList.sHeader = ServerPacketHeader::SMSG_SERVER_LIST;
        strcpy(serverList.sHeaderStruct.sServerName, fields->GetString(4).c_str());
        serverList.sHeaderStruct.sTime = GetUnixTimeStamp();
        serverList.sHeaderStruct.sTicket = Maths::GetRandomNumber(1, 1000);
        serverList.sHeaderStruct.sUnknown = 0;
        serverList.sHeaderStruct.sClanServerIndex = 0; // TOOD; What is this?
        serverList.sHeaderStruct.sGameServers = fields->GetRowCount();

        uint8 counter = 0;
        do
        {
            // Client requires us to send this 3 times... original source does this aswell
            strcpy(serverList.sServersStruct[counter].sName, fields->GetString(5).c_str());
            strcpy(serverList.sServersStruct[counter].sIP[0], fields->GetString(2).c_str());
            strcpy(serverList.sServersStruct[counter].sIP[1], fields->GetString(2).c_str());
            strcpy(serverList.sServersStruct[counter].sIP[2], fields->GetString(2).c_str());

            serverList.sServersStruct[counter].sPort[0] = 10010;
            serverList.sServersStruct[counter].sPort[1] = 10010;
            serverList.sServersStruct[counter].sPort[2] = 10010;
            serverList.sServersStruct[counter].sPort[3] = 0;

            counter++;
        } while (fields->GetNextResult());

        serverList.sLength += ((sizeof(PacketServerList::Server) * serverList.sHeaderStruct.sGameServers) + (sizeof(PacketServerList::Server) * (0)));
        serverList.sEncKeyIndex = 0;
        serverList.sEncrypted = 1;
        SendPacket((uint8*)(Packet*)&serverList, serverList.sLength);
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//