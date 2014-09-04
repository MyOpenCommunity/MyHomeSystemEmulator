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

#ifndef JSONHANDLER_H
#define JSONHANDLER_H

//  QJson libary could help parsing JSON frames
//  http://qjson.sourceforge.net/usage.html

class QByteArray;
class QVariant;

class JsonHandler
{
  public:
    static JsonHandler& getInstance();
    bool FromJsonToStruc(const QByteArray& theJson, QVariant& theSrt);
    bool FromStructToJson(const QVariant& theSrt, QByteArray& theJson);


  private:
    JsonHandler();
    JsonHandler(const JsonHandler&);
    JsonHandler& operator=(const JsonHandler&);
};

#endif // JSONHANDLER_H
