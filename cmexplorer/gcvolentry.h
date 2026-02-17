#pragma once

#include "volentry.h"

class GCVolEntry : public VolEntry
{
public:
    GCVolEntry(uint32_t index, QByteArray data);

    QImage image() override;
    QString to_string() override;

private:
    QImage m_image;
};