// updatable.cpp

#include "entity/inheritances/updatable.h"

updatable::updatable(bool p_active)
    : m_active(p_active)
{
}

void updatable::set_active(bool p_active)
{
    if(!m_active != !p_active)
    {
        switch(p_active)
        {
            case true:  on_enable();    break;
            case false: on_disable();   break;
                break;
        }
    }
}
bool updatable::get_active() { return false; }