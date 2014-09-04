/* This file is part of My Home System Emulator
 *
 * My Home System Emulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * My Home System Emulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with My Home System Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "f422status.h"

const int F422Status::MAX_I3 = 0;
const int F422Status::MAX_I4 = 9;
const int F422Status::MIN_I3 = 0;
const int F422Status::MIN_I4 = 1;

F422Status::F422Status()
{
    m_I3 = "0";
    m_I4 = "";
}

F422Status::F422Status(QString id)
{
    m_I3 = "0";
    m_I4 = "";
    m_id = id;
}
