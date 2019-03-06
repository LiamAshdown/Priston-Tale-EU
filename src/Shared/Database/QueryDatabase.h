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

#ifndef _PristonTale_QueryDatabase_h_
#define _PristonTale_QueryDatabase_h_
#include "../Common/SharedDefines.h"
#include "Database.h"
#include "Fields.h"

namespace Priston
{
    class QueryAuthDatabase
    {
    public:
        QueryAuthDatabase() {}
        ~QueryAuthDatabase() {}

    public:
        void Query(std::string query)
        {
            std::shared_ptr<MySQLConnection> connection = sDatabase->GetConnectionPool()->Borrow();
            try
            {
                std::shared_ptr<sql::Connection> sqlConnection = connection->SQLConnection;
                std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(sqlConnection->createStatement());
                mResultSet = std::shared_ptr<sql::ResultSet>(statement->executeQuery(query.c_str()));
                mField.mResultSet = mResultSet;
            }
            catch (sql::SQLException &e)
            {
                sDatabase->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
            }
        }

        Field& GetResult()
        {
            return mField;
        }

    private:
        std::shared_ptr<sql::ResultSet> mResultSet;
        Field mField;
    }; 
}

#endif /* !_PristonTale_QueryDatabase_h_ */
