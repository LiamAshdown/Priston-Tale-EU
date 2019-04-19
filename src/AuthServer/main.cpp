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
#include "Common/SharedDefines.h"
#include "Config/Config.h"

bool LoadDatabase();

int main()
{
    /// Initialize our logger
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    LOG_NONE << " ______     ______   ______     ______     ______     ______     ______   ______     __   __     ______ ";
    LOG_NONE << "/\\  ___\\   /\\__  _\\ /\\  ___\\   /\\  ___\\   /\\  == \\   /\\  ___\\   /\\__  _\\ /\\  __ \\   /\\ ' - .\\ \ / \\  ___\\     ";
    LOG_NONE << "\\ \\___  \\  \\/_/\\ \\/ \\ \\  __\\   \\ \\  __\\   \\ \\  __<   \\ \\___  \\  \\/_/\\ \\/ \\ \\ \\/\\ \\  \\ \\ \\-.  \\  \\ \\  __\\     ";
    LOG_NONE << " \\/\\_____\\    \\ \\_\\  \\ \\_____\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\/\\_____\\    \\ \\_\\  \\ \\_____\\  \\ \\_\\\\'\\_\\  \\ \\_____\\   ";
    LOG_NONE << "  \\/_____/     \\/_/   \\/_____/   \\/_____/   \\/_/ /_/   \\/_____/     \\/_/   \\/_____/   \\/_/ \\/_/   \\/_____/    ";
    LOG_NONE << "                                                                    Powered by Boost & MySQL";


    if (!sConfig->SetFile("authserver.conf"))
    {
        LOG_ERROR << "Could not find configuration file authserver.conf";
        LOG_ERROR << "Shutting Down....";
        Sleep(5000);
        return -1;
    }
    else
        LOG_INFO << "Successfully loaded configuration file!";

    sOpcode->InitializePackets();

    if (!LoadDatabase())
    {
        LOG_ERROR << "Could not load database";
        LOG_ERROR << "Shutting Down....";
        Sleep(5000);
        return -1;
    }

    SteerStone::Listener<SteerStone::AuthSocket> listener(sConfig->GetStringDefault("BindIP", "127.0.0.1"), sConfig->GetIntDefault("AuthServerPort", DEFAULT_AUTH_PORT), 
        sConfig->GetIntDefault("NetworkThreadProcessors", 1));

    LOG_INFO << "Successfully listening on " << sConfig->GetStringDefault("BindIP", "127.0.0.1") << " and binded on " << sConfig->GetIntDefault("AuthServerPort", DEFAULT_AUTH_PORT);
    
    /// Keep application alive
    while (true){}

    return 0;
}

bool LoadDatabase()
{
    if (sDatabase->CreateDatabase(sConfig->GetStringDefault("AuthDatabaseInfo").c_str(), sConfig->GetIntDefault("AuthDatabase.ConnectionPool", 10)))
        return true;
    else
        return false;
}