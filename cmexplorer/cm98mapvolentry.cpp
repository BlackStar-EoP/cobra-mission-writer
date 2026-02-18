#include "cm98mapvolentry.h"

#include <QPainter>

#include "spritevolentry.h"

#include "gcvolentry.h"
#include "volfile.h"

CM98MapVolEntry::CM98MapVolEntry(uint32_t index, const QString& name, QByteArray data, VolFile* cm98_vol_file)
: VolEntry(index, data)
, m_cm98_vol_file(cm98_vol_file)
, m_name(name)
{
    build_tileset();
    create_img();
}

QImage CM98MapVolEntry::image()
{
    return m_map;
}

QString CM98MapVolEntry::to_string()
{
    return m_name;
}

void CM98MapVolEntry::next()
{

}

void CM98MapVolEntry::prev()
{

}

void CM98MapVolEntry::check(bool checked)
{

}

void CM98MapVolEntry::build_tileset()
{
    GCVolEntry* tileset = nullptr;
    if (m_cm98_vol_file != nullptr)
    {
        for (VolEntry* entry : m_cm98_vol_file->entries())
        {
            GCVolEntry* gc = static_cast<GCVolEntry*>(entry);
            if (m_name.contains(gc->to_string()))
            {
                tileset = gc;
            }
        }
    }

    if (tileset != nullptr)
    {
        QImage tiles = tileset->image();
        for (uint32_t y = 0u; y < 480; y += SpriteVolEntry::TILE_HEIGHT)
        {
            for (uint32_t x = 0u; x < 640; x += SpriteVolEntry::TILE_WIDTH)
            {
                QImage tile(SpriteVolEntry::TILE_WIDTH, SpriteVolEntry::TILE_HEIGHT, QImage::Format_ARGB32);
                tile = tiles.copy(x, y, SpriteVolEntry::TILE_WIDTH, SpriteVolEntry::TILE_HEIGHT);
                m_tileset.push_back(tile);
            }
        }
    }
}

void CM98MapVolEntry::create_img()
{
    if (m_tileset.isEmpty())
        return;

    uint8_t* d = (uint8_t*)m_data.constData();
    uint16_t offset = 0u;

    QImage map(MAP_WIDTH * SpriteVolEntry::TILE_WIDTH, MAP_HEIGHT * SpriteVolEntry::TILE_HEIGHT, QImage::Format_ARGB32);
    map.fill(Qt::red);

    QPainter p(&map);

    uint32_t stride = 0u;

    for (uint32_t y = 0u; y < MAP_HEIGHT; ++y)
    {
        for (uint32_t x = 0u; x < MAP_WIDTH; ++x)
        {
            uint8_t tile = d[stride + (MAP_HEIGHT * x)];

            if (tile >= m_tileset.size()) continue;
            p.drawImage(x * SpriteVolEntry::TILE_WIDTH, y * SpriteVolEntry::TILE_HEIGHT, m_tileset[tile]);
        }
        ++stride;
    }

    m_map = map;
}