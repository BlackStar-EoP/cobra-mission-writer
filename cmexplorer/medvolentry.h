#pragma once

#include "volentry.h"

class VolFile;

class MedVolEntry : public VolEntry
{
public:
    MedVolEntry(uint32_t index, QByteArray data, QVector<VolFile*>& vol_files);

    QImage image() override;
    QString to_string() override;
    void next() override;
    void prev() override;
    void check(bool checked) override;

private:
    void parse_map();
    void create_img();

private:
    QVector<VolFile*>& m_vol_files;
    QImage m_map;
    QString m_tileset;
    QString m_name;
    VolFile* m_sprite_vol = nullptr;

    uint16_t m_map_data = 0u;
    uint16_t m_width = 0u;
    uint16_t m_height = 0u;
    uint16_t m_floors = 0u;

    uint16_t m_current_floor = 0u;
    bool m_render_triggers = false;

};