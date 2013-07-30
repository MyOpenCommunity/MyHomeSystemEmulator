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
