/*
* Priston Tale EU
* Copyright(C) 2019
*
* This program is free software : you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
*(at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
*but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see < http://www.gnu.org/licenses/>.
*/

#ifndef _PristonTale_Database_h_
#define _PristonTale_Database_h_
#include "../Common/SharedDefines.h"
#include "cppconn/exception.h"
#include "mysql_connection.h"
#include "MYSQLConnection.h"

namespace Priston
{
    class Database
    {
    public:
        static Database* instance();

    public:
        Database();
        ~Database();

    public:
        bool InitializeConnectionPool(const char* infoString, const uint32 poolSize);
        void PrintException(sql::SQLException &e, char* file, char* function, uint32 line);

        MySQLConnectionFactory* GetConnectionFactory();
        ConnectionPool<MySQLConnection>* GetConnectionPool();

    private:
        MySQLConnectionFactory* mConnectionFactory;
        ConnectionPool<MySQLConnection>* mPool;

        std::string mUsername;
        std::string mPassword;
        std::string mPort;
        std::string mHost;
        std::string mDatabase;
        uint32 mPoolSize;
    };
}
#define sDatabase Priston::Database::instance()

#endif /* !_PristonTale_Database_h_ */
