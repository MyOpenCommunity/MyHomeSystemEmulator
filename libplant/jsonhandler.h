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
