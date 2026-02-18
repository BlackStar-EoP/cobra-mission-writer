#pragma once

#include "volentry.h"

class GCVolEntry : public VolEntry
{
public:
    GCVolEntry(uint32_t index, const QString& name, QByteArray data);

    QImage image() override;
    QString to_string() override;

    void next() override;
    void prev() override;
    void check(bool checked) override;

private:
    QVector<QImage> m_frames;
    uint32_t m_frame_idx = 0u;
    QString m_name;
};