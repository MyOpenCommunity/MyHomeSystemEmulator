#ifndef EVENTSOURCE_H
#define EVENTSOURCE_H

class EventHandler;
class SysError;


/** Interface Class EventHandle Inteface definition
*
*   Every event source component has to refine this interface
*   according to the framework conventions
*
*/
class EventSource
{
  public:

    /** Pure virtual service for event handling registration
    *   @param[in]  theHnd    event handler to register
    *   @param[in]  theErr    error indication
    */
    virtual void  registerEventHnd(EventHandler* theHnd, SysError& theErr)=0;


    /** Pure virtual service for event handling deregistration
    *   @param[in]  theHnd    event handler to unregister
    */
    virtual void  unRegisterEventHnd(EventHandler* theHnd)=0;

};

#endif // EVENTSOURCE_H
