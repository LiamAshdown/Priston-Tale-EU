#include "AuthSocket.h"
#include "Common/SHA1.h"
#include "Config/Config.h"
#include "Common/Maths.h"
#include "Common/Timer.h"

namespace SteerStone
{
    /// HandleLoginUser
    /// Checks Login data and sends back result to client
    /// @p_Packet : Packet sent by client
    void AuthSocket::HandleLoginUser(const Packet* p_Packet)
    {
        PacketLoginUser* l_PacketUser = (PacketLoginUser*)p_Packet;

        QueryDatabase l_Database("auth");
        l_Database.PrepareQuery("SELECT id, user_name, sha_pass FROM account WHERE user_name = ?");
        l_Database.GetStatement()->setString(1, static_cast<std::string>(l_PacketUser->UserID).c_str());
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
        {
            PacketAccountLoginCode l_AccountLogin;
            l_AccountLogin.Length       = sizeof(l_AccountLogin);
            l_AccountLogin.PacketHeader = ServerPacketHeader::SMSG_ACCOUNT_LOGIN_CODE;
            l_AccountLogin.Reserved     = 0;
            l_AccountLogin.Code         = LoginError::ACCOUNTLOGIN_IncorrectName;
            l_AccountLogin.FailCode     = 1;
            l_AccountLogin.EncKeyIndex  = 0;
            l_AccountLogin.Encrypted    = 1;
            SendPacket((uint8*)(Packet*)&l_AccountLogin, l_AccountLogin.Length);
            return;
        }

        Result* l_Result = l_Database.Fetch();

        if (CalculateSHA1Hash(boost::to_upper_copy<std::string>(static_cast<std::string>(l_PacketUser->UserID)
            + ":" + static_cast<std::string>(l_PacketUser->Password))) != l_Result->GetString(3))
        {
            PacketAccountLoginCode l_AccountLogin;
            l_AccountLogin.Length       = sizeof(l_AccountLogin);
            l_AccountLogin.PacketHeader = ServerPacketHeader::SMSG_ACCOUNT_LOGIN_CODE;
            l_AccountLogin.Reserved     = 0;
            l_AccountLogin.Code         = LoginError::ACCOUNTLOGIN_IncorrectPassword;
            l_AccountLogin.FailCode     = 1;
            l_AccountLogin.EncKeyIndex  = 0;
            l_AccountLogin.Encrypted    = 1;

            SendPacket((uint8*)(Packet*)&l_AccountLogin, l_AccountLogin.Length);
            return;
        }

        PacketChecksumFunctionList l_PacketCheck;
        l_PacketCheck.Length           = sizeof(PacketChecksumFunctionList);
        l_PacketCheck.PacketHeader     = ServerPacketHeader::SMSG_CHECK_SUM;
        l_PacketCheck.Key              = CHECK_SUM_PACKET;
        l_PacketCheck.EncKeyIndex      = 0;
        l_PacketCheck.Encrypted        = 1;
        SendPacket((uint8*)(Packet*)&l_PacketCheck, l_PacketCheck.Length);

        /// Update database that user is online and last logged in date
        l_Database.ClearParameters();
        l_Database.PrepareQuery("UPDATE account SET online = 1, last_online = NOW() WHERE id = ?");
        l_Database.GetStatement()->setUInt(1, l_Result->GetUint32(1));
        l_Database.ExecuteQuery();

        SendUserSuccess(l_Result);
    }

    /// SendUserSuccess
    /// Client can now login into the game server
    /// @p_Result : Database result
    void AuthSocket::SendUserSuccess(Result* p_Result)
    {
        PacketUserInfo l_PacketUser;
        l_PacketUser.Length             = sizeof(PacketUserInfo);
        l_PacketUser.PacketHeader       = ServerPacketHeader::SMSG_USER_INFO;
        strcpy(l_PacketUser.UserID, p_Result->GetString(2).c_str());
        l_PacketUser.CharCount          = 0; ///< TODO; Character Count
        l_PacketUser.EncKeyIndex        = 0;
        l_PacketUser.Encrypted          = 1;
        SendPacket((uint8*)(Packet*)&l_PacketUser, l_PacketUser.Length);

        QueryDatabase l_Database("auth");
        l_Database.PrepareQuery("SELECT id, ip_address, port, server_name, realm_name FROM server_realms");
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        p_Result = l_Database.Fetch();

        PacketServerList l_ServerList;
        l_ServerList.Length                         = sizeof(Packet) + sizeof(PacketServerList::Header);
        l_ServerList.PacketHeader = ServerPacketHeader::SMSG_SERVER_LIST;
        strcpy(l_ServerList.HeaderStruct.ServerName, p_Result->GetString(4).c_str());
        l_ServerList.HeaderStruct.Time              = Maths::GetUnixTimeStamp();
        l_ServerList.HeaderStruct.Ticket            = Maths::GetRandomUint32(1, 1000); ///< Ticket Id, not sure what this is
        l_ServerList.HeaderStruct.Unknown           = 0;
        l_ServerList.HeaderStruct.ClanServerIndex   = 0; ///< TODO; Find out what this is
        l_ServerList.HeaderStruct.GameServers       = p_Result->GetRowCount();

        uint16 l_Counter = 0;
        do
        {
            /// TOOD; Find out why we need to do this 3 times
            strcpy(l_ServerList.ServersStruct[l_Counter].Name, p_Result->GetString(5).c_str());
            strcpy(l_ServerList.ServersStruct[l_Counter].IP[0], p_Result->GetString(2).c_str());
            strcpy(l_ServerList.ServersStruct[l_Counter].IP[1], p_Result->GetString(2).c_str());
            strcpy(l_ServerList.ServersStruct[l_Counter].IP[2], p_Result->GetString(2).c_str());

            l_ServerList.ServersStruct[l_Counter].Port[0] = DEFAULT_WORLD_PORT;
            l_ServerList.ServersStruct[l_Counter].Port[1] = DEFAULT_WORLD_PORT;
            l_ServerList.ServersStruct[l_Counter].Port[2] = DEFAULT_WORLD_PORT;
            l_ServerList.ServersStruct[l_Counter].Port[3] = DEFAULT_WORLD_PORT;

            l_Counter++;
        } while (p_Result->GetNextResult());

        l_ServerList.Length         += ((sizeof(PacketServerList::Server) * l_ServerList.HeaderStruct.GameServers) + (sizeof(PacketServerList::Server) * (0)));
        l_ServerList.EncKeyIndex    = 0;
        l_ServerList.Encrypted      = 1;
        SendPacket((uint8*)(Packet*)&l_ServerList, l_ServerList.Length);
    }

    /// HandlePing
    /// Keep connection alive by client and server
    /// @p_Packet : Packet sent by client
    void AuthSocket::HandlePing(const Packet* p_Packet)
    {
        if (((PacketPing*)&p_Packet)->Length != sizeof(PacketPing))
            return;

        /// Check last pong time
        /// TODO; Add protection against Slowloris Attack
        if ((sTimer->GetServerTime() - p_PingDiff) > sConfig->GetIntDefault("PongTimer", 30000))
        {
            CloseSocket();
            return;
        }

        ((PacketPing*)&p_Packet)->Tick = GetTickCount();
        SendPacket((uint8*)&p_Packet, p_Packet->Length);
    }
} ///< NAMESPACE STEERSTONE