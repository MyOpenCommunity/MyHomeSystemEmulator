#include "gwtstatus.h"

GwtStatus::GwtStatus()
{
    m_id = "";
    m_configured = false;
    m_password = "";
}


GwtStatus::GwtStatus(QString id) {
    m_id = id;
    m_configured = false;
}
