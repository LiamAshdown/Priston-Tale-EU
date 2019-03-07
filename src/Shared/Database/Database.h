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
    typedef struct DatabaseStruct
    {
    public:
        DatabaseStruct() : sConnectionFactory(nullptr), sPool(nullptr) {}
        ~DatabaseStruct()
        {
            delete sConnectionFactory;
            delete sPool;
        }

        ConnectionPool<MySQLConnection>* GetConnectionPool()
        {
            return sPool;
        }

        MySQLConnectionFactory* sConnectionFactory;
        ConnectionPool<MySQLConnection>* sPool;

        std::string sUsername;
        std::string sPassword;
        std::string sPort;
        std::string sHost;
        std::string sDatabaseName;
        uint32 sPoolSize;

    }DatabaseHolder;

    typedef std::map<std::string, DatabaseHolder*> DatabaseMap;

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

        DatabaseHolder* GetDatabase(const std::string& database);
        void RemoveDatabase(const std::string& database);

    private:
        DatabaseMap mDatabaseCont;
    };
}
#define sDatabase Priston::Database::instance()

#endif /* !_PristonTale_Database_h_ */
