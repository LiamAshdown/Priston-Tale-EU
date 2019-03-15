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

#ifndef _Priston_QueryDatabase_h_
#define _Priston_QueryDatabase_h_
#include "../Common/SharedDefines.h"
#include "Database.h"
#include "Fields.h"

namespace Priston
{
    class QueryDatabase
    {
    public:
       explicit QueryDatabase(const std::string database);
        ~QueryDatabase();

    public:
        // Direct Execute
        void DirectExecuteQuery(const std::string& query);

        // Prepare Query
        void PrepareQuery(const std::string& query);
        void ExecuteQuery();
        bool GetResult();
        std::shared_ptr<sql::PreparedStatement>& GetStatement();

        // Neutral
        void Release();
        Field* Fetch();

    private:
        // Database
        std::string mDatabase;
        std::shared_ptr<MySQLConnection> mConnection;
        std::shared_ptr<sql::Connection> mSqlConnection;

        // Direct Execute
        std::shared_ptr<sql::Statement> mStatement;

        // Prepare Query
        std::shared_ptr<sql::PreparedStatement> mPreparedStatement;

        // Neutral
        bool mExecuteResult;
        bool mIsExecuteResult;
        bool mHasReleased;
        std::unique_ptr<sql::ResultSet> mResultSet;
        Field mField;
    }; 
}

#endif /* !_Priston_QueryDatabase_h_ */
