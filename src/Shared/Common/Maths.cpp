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

#include "Maths.h"

namespace SteerStone
{
    namespace Maths
    {
        static std::default_random_engine s_Random(time(NULL));

        /// GetRandomNumber
        /// Return a random number in specified range
        uint32 GetRandomUint32(uint32 const p_Min, uint32 const p_Max)
        {
            std::uniform_int_distribution<uint32> l_Random(p_Min, p_Max);
            return l_Random(s_Random);
        }

        /// GetUnitTimeStamp
        /// Returns time in unix
        uint64 GetUnixTimeStamp()
        {
            time_t l_Time = std::time(0);
            return static_cast<long unsigned int>(l_Time);
        }
    } ///< NAMESPACE MATHS
} ///< NAMESPACE STEERSTONE
