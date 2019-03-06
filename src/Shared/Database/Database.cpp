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
#include "Database.h"
//-----------------------------------------------//
namespace Priston
{
    //-----------------------------------------------//
    Tokens StrSplit(const std::string& src, const std::string& sep);
    //-----------------------------------------------//
    Database* Database::instance()
    {
        static Database instance;
        return &instance;
    }
    //-----------------------------------------------//
    Database::Database()
    {
        mConnectionFactory = nullptr;
        mPool = nullptr;
    }
    //-----------------------------------------------//
    Database::~Database()
    {
        delete mConnectionFactory;
        delete mPool;
    }
    //-----------------------------------------------//
    MySQLConnectionFactory* Database::GetConnectionFactory()
    {
        return mConnectionFactory;
    }
    //-----------------------------------------------//
    ConnectionPool<MySQLConnection>* Database::GetConnectionPool()
    {
        return mPool;
    }
    //-----------------------------------------------//
    bool Database::InitializeConnectionPool(const char* infoString, const uint32 poolSize)
    {
        try 
        {
            std::string username, password, database, host;
            std::string port; 

            Tokens tokens = StrSplit(infoString, ";");

            Tokens::iterator iter = tokens.begin();

            if (iter != tokens.end())
                host = *iter++;
            if (iter != tokens.end())
                port = *iter++;
            if (iter != tokens.end())
                username = *iter++;
            if (iter != tokens.end())
                password = *iter++;
            if (iter != tokens.end())
                database = *iter++;
            
            mUsername = username;
            mPassword = password;
            mDatabase = database;
            mHost = host;
            mPort = port;
            mPoolSize = poolSize;

            mConnectionFactory = new MySQLConnectionFactory(mUsername, mPassword, mDatabase, mHost, mPort);
            mPool = new ConnectionPool<MySQLConnection>(poolSize, mConnectionFactory);
            return true;
        }
        catch (sql::SQLException &e) 
        {
            this->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
            return false;
        }
    }
    //-----------------------------------------------//
    void Database::PrintException(sql::SQLException& e, char* file, char* function, uint32 line)
    {
        std::string message = e.what();

        LOG_ERROR << message << std::endl;

        if (message.find("has gone away") != std::string::npos) 
        {
            if (mConnectionFactory != nullptr)
            {
                delete mConnectionFactory;
                mConnectionFactory = new MySQLConnectionFactory(mUsername, mPassword, mDatabase, mHost, mPort);
            }

            if (mPool != nullptr)
            {
                delete mPool;
                mPool = new ConnectionPool<MySQLConnection>(mPoolSize, mConnectionFactory);
            }
        }
    }
    //-----------------------------------------------//
    Tokens StrSplit(const std::string& src, const std::string& sep)
    {
        Tokens r;
        std::string s;
        for (char i : src)
        {
            if (sep.find(i) != std::string::npos)
            {
                if (s.length()) r.push_back(s);
                s.clear();
            }
            else
            {
                s += i;
            }
        }
        if (s.length()) r.push_back(s);
        return r;
    }
    //-----------------------------------------------//
}