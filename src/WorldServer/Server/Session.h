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

#ifndef _PristonTale_Session_h_
#define _PristonTale_Session_h_
#include "Common/SharedDefines.h"
#include "Opcode/Opcodes.h"
#include <mutex>

namespace Priston
{
    class WorldSocket;

    class Session
    {
    public:
        friend class WorldSocket;

    public:
        Session(WorldSocket* worldSocket);
        ~Session();

    private:
        void QueuePacket(std::unique_ptr<Packet> packet);

    private:
        std::shared_ptr<WorldSocket> mSocket;
        std::mutex mMutex;

        std::deque<std::unique_ptr<Packet>> mRecvQueue;
    };
}

#endif /* _PristonTale_Session_h_ */