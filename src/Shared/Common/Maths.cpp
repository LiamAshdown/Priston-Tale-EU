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
#include "Maths.h"
#include <random>
//-----------------------------------------------//
namespace Priston
{
    //-----------------------------------------------//
    namespace Maths
    {
        int GetRandomNumber(int min, int max)
        {
            if (min == max)
                return min;

            int i = rand();

            i += i << 16;

            return (i % (min - max)) + min;
        }
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//