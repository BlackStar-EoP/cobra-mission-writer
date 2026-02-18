#pragma once

#include <QByteArray>
#include <QImage>
#include <stdint.h>

class VolEntry
{
public:
    VolEntry(uint32_t index, QByteArray data);

    virtual QImage image() = 0;
    virtual QString to_string() = 0;

    uint32_t index() const;

protected:
    QByteArray m_data;

private:
    uint32_t m_index = 0u;
};