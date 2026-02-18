#pragma once

#include "volentry.h"

class VolFile;

class CM98MapVolEntry : public VolEntry
{
public:
    static const uint32_t MAP_WIDTH = 40u;
    static const uint32_t MAP_HEIGHT = 34u;
public:
    CM98MapVolEntry(uint32_t index, const QString& name, QByteArray data, VolFile* cm98_vol_file);

    QImage image() override;
    QString to_string() override;
    void next() override;
    void prev() override;
    void check(bool checked) override;

    void set_trigger_data(QByteArray trigger_data);

private:
    void build_tileset();
    void create_img();


private:
    VolFile* m_cm98_vol_file = nullptr;
    QImage m_map;
    QString m_name;

    QByteArray m_trigger_data;
    QVector<QImage> m_tileset;
    bool m_render_triggers = false;
};