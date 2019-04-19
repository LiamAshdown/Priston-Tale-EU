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
#include "Network/Listener.h"
#include "Common/SharedDefines.h"
#include "Config/Config.h"
#include "Database/Database.h"

bool StartUpDataBase();

int main()
{
    // Initialize our logger
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    if (!sConfig->SetSource("worldserver.conf"))
    {
        LOG_ERROR << "Could not find configuration file worldserver.conf";
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

    sOpcode->InitializePackets();

    Priston::Listener<Priston::WorldSocket> listener(sConfig->GetStringDefault("BindIP", "127.0.0.1"), sConfig->GetIntDefault("AuthServerPort", DEFAULT_WORLD_PORT), 
        sConfig->GetIntDefault("NetworkThreadProcessors", 1));

    LOG_INFO << "Successfully listening on " << sConfig->GetStringDefault("BindIP", "127.0.0.1") << " and binded on " << sConfig->GetIntDefault("AuthServerPort", DEFAULT_WORLD_PORT);
    
    while (true)
    {

    }

    return 0;
}

bool StartUpDataBase()
{
    std::string dbstring = sConfig->GetStringDefault("WorldDatabaseInfo");
    if (dbstring.empty())
    {
        LOG_ERROR << "Database not specified";
        return false;
    }

    if (!sDatabase->InitializeConnectionPool(dbstring.c_str(), sConfig->GetIntDefault("WorldDatabase.WorkerThreads", 1)))
    {
        LOG_ERROR << "Couldn't connect to the database...";
        return false;
    }
    else
    {
        LOG_INFO << "Successfully connected to MYSQL...";
    }

    return true;
}