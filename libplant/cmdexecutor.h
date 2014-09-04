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

#ifndef CMDEXECUTOR_H
#define CMDEXECUTOR_H

class SysError;
class QString;

/** Interface Class Command Executor Inteface definition
*
*   Every command executor have to refine this interface to be able to execute commands
*   according to the framework conventions
*
*/
class CmdExecutor
{
  public:
    /** Command priority Enum
    *
    */
    enum CMD_PRIO {
        LOW=0,      /**< enum value LOW = for low prio commands */
        NORMAL,     /**< enum value NORMAL = for normal prio commands */
        HIGH        /**< enum value HIGH = for high prio commands */
    };

    /** Pure virtual service for command execution
    *   @param[in]  thePrio     command priority
    *   @param[in]  theCmd      the command plus parameter structure (e.g. JSON stream) that has to be executed
    *   @param[out] theResult   command result (e.g. JSOAN stream)
    *   @param[out] theErr      the command error
    *   @return     the command ID that will be used for subsequent operation on a submitted command
    */
    virtual unsigned int executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr)=0;


    /** Pure virtual service for command execution
    *   @param[in]  theCmdId    Command
    *   @return     the operation result
    */
    virtual bool cancelCmd(unsigned int theCmdId)=0;
};

#endif // CMDEXECUTOR_H
