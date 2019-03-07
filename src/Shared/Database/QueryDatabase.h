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
    class QueryDatabase
    {
    public:
        QueryDatabase(const std::string database)
        {
            mDatabase = database;
            mConnection = sDatabase->GetDatabase(mDatabase)->GetConnectionPool()->Borrow();
            mSqlConnection = mConnection->SQLConnection;
        }
        ~QueryDatabase()
        {
            sDatabase->GetDatabase(mDatabase)->GetConnectionPool()->UnBorrow(mConnection);
        }

    public:
        void QueryExecute(const std::string& query)
        {
            try
            {
                std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(mSqlConnection->createStatement());
                mResultSet = std::shared_ptr<sql::ResultSet>(statement->executeQuery(query.c_str()));
                mField.mResultSet = mResultSet;
            }
            catch (sql::SQLException &e)
            {
                sDatabase->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
            }
        }

        void PreparedStatementQuery(const std::string& query)
        {
            try
            {
                mPrepareStatement = std::shared_ptr<sql::PreparedStatement>(mSqlConnection->prepareStatement(query.c_str()));
            }
            catch (sql::SQLException &e)
            {
                sDatabase->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
            }
        }

        std::shared_ptr<sql::PreparedStatement>& GetStatement()
        {
            return mPrepareStatement;
        }

        void ExecuteQuery()
        {
            mResultSet = std::shared_ptr<sql::ResultSet>(mPrepareStatement->executeQuery());
        }

        bool RecordExists()
        {
            if (mResultSet->next())
            {
                mField.mResultSet = mResultSet;
                return true;
            }
            return false;
        }

        Field* GetResult()
        {
            return &mField;
        }

    public:
        std::string mDatabase;
        std::shared_ptr<sql::ResultSet> mResultSet;
        std::shared_ptr<MySQLConnection> mConnection;
        std::shared_ptr<sql::Connection> mSqlConnection;
        std::shared_ptr<sql::PreparedStatement> mPrepareStatement;
        Field mField;
    }; 
}

#endif /* !_PristonTale_QueryDatabase_h_ */
