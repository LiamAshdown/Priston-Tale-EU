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
#include "Network/Listener.h"
#include "Common/SharedDefines.h"
#include "Config/Config.h"
#include "Database/Database.h"
//-----------------------------------------------//
bool StartUpDataBase();
//-----------------------------------------------//
int main()
{
    // Initialize our logger
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    if (!sConfig->SetSource("authserver.conf"))
    {
        LOG_ERROR << "Could not find configuration file authserver.conf";
        Sleep(5000);
        return 1;
    }
    else
        LOG_INFO << "Successfully found configuration file!";

    if (!StartUpDataBase())
    {
        LOG_ERROR << "Shutting Down....";
        Sleep(5000);
        return -1;
    }

    Priston::Listener<Priston::AuthSocket> listener(sConfig->GetStringDefault("BindIP", "127.0.0.1"), sConfig->GetIntDefault("AuthServerPort", DEFAULT_AUTH_PORT), 
        sConfig->GetIntDefault("NetworkProcessors", 1));

    LOG_INFO << "Successfully listening on " << sConfig->GetStringDefault("BindIP", "127.0.0.1") << " and binded on " << sConfig->GetIntDefault("AuthServerPort", DEFAULT_AUTH_PORT);

    sOpcode->InitializePackets();
    LOG_INFO << "Opcodes sucessfully loaded!";

    
    while (true)
    {

    }

    return 0;
}
//-----------------------------------------------//
bool StartUpDataBase()
{
    std::string dbstring = sConfig->GetStringDefault("AuthDatabaseInfo");
    if (dbstring.empty())
    {
        LOG_ERROR << "Database not specified";
        return false;
    }

    if (!sDatabase->InitializeConnectionPool(dbstring.c_str(), sConfig->GetIntDefault("AuthDatabase.WorkerThreads", 1)))
    {
        LOG_ERROR << "Couldn't connect to the database...";
        return false;
    }
    else
    {
        LOG_INFO << "Successfully connected to MYSQL...";
        LOG_INFO << "MYSQL Worker Threads: " << sConfig->GetIntDefault("AuthDatabase.WorkerThreads", 1);
    }

    return true;
}