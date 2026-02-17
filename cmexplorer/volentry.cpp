#include "volentry.h"

VolEntry::VolEntry(uint32_t index, QByteArray data)
: m_index(index)
, m_data(data)
{
}

uint32_t VolEntry::index() const
{
    return m_index;
}