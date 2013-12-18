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
