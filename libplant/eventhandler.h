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

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class QByteArray;


/** Interface Class EventHandle Inteface definition
*
*   Every event handler component has to refine this interface
*   according to the framework conventions
*
*/
class EventHandler
{

  public:
    enum CMD_PRIO {
        LOW=0,      /**< enum value LOW = for low prio commands */
        NORMAL,     /**< enum value NORMAL = for normal prio commands */
        HIGH        /**< enum value HIGH = for high prio commands */
    };

    /** Pure virtual service for event handling
    *   @param[in]  thePrio     command priority
    *   @param[in]  theEvent    the command plus parameter structure (e.g. JSON stream) that has to be executed

    */
    virtual void handleEvent(CMD_PRIO thePrio, const QByteArray& theEvent)=0;
};

#endif // EVENTHANDLER_H
