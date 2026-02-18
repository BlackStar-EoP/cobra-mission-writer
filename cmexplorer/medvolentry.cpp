#include "medvolentry.h"

#include <QPainter>

#include "spritevolentry.h"
#include "volfile.h"

MedVolEntry::MedVolEntry(uint32_t index, QByteArray data, QVector<VolFile*>& vol_files)
: VolEntry(index, data)
, m_vol_files(vol_files)
{
    parse_map();
    create_img();
}

QImage MedVolEntry::image()
{
    return m_map;
}

QString MedVolEntry::to_string()
{
    return m_name;
}

void MedVolEntry::parse_map()
{
    uint8_t* d = (uint8_t*)m_data.constData();
    /*
MD Entry
Each MD entry inside MED.VOL consists of 5 blocks. The header (96 bytes), a block of unknown data (variable size), the tiledata for all the floors(dependent on size and number of floors), the triggers for all floors (same as the floor sizes, but with most entries 0) and a footer of 256 bytes.

MD Header
0 - 6 : signature 4D 44 03 30 02 02 14
7 - 9 : Unknown, most of the times 77 60 00, but values 9F 60 00, FF 60 00 and 8B 60 00 are also found in a few maps
0A - 0B : UINT16LE, the offset inside this MD chunk where the actual map data starts
0C - 0F : UINT32LE, the size of this MD chunk
10 - 11 : UINT16LE, the width (in tiles) of this map
12 - 13 : UINT16LE, the height (in tiles) of this map
14 - 15 : UINT16LE, the number of floors in this map
16 - 1F : Character array, refers to the used tileset (values can be CASTLE, AREA1, AREA2 etc. See MCG.VOL), rest zeroes
20 - 3F : Palette data, 16 colors, each color stored as UINT16LE, colors are stored as 4 bit nibbles.
40 - 5F : Character array, contains 2 names for the map, or for HOUSE types, the area that is referred. A long and a short version seperated by a pipe character.
Unknown data block
Unknown data block, usually ranging from 0060 to 0420. The data inside looks the same for all the MD chunks. It looks like a lot of 16 bit integers.

Tile data block
The size of the block is width * height * number of floors. Each byte corresponds to the tile number in the used tileset. The floors are stored sequentially.

Triggers block
The same layout as the tile data block, but now most bytes are 00 and the bytes that are not are triggers such as item pickups, stairs, doors etc. Unknown what the numbers mean, but exits are usually 0x20, item pickups range in 0x40 and character sprites are usually 0xC0 and higher.

Footer
At the end of each MD chunk there are 256 bytes, unknown what the data represents, but it looks like it is related to the trigger block.
    */
    m_map_data = *(uint16_t*)(d + 0x0A);
    m_width = *(uint16_t*)(d + 0x10);
    m_height = *(uint16_t*)(d + 0x12);
    m_floors = *(uint16_t*)(d + 0x14);

    for (uint32_t c = 0x16; c < 0x1F; ++c)
    {
        if (d[c] == 0) break;

        m_tileset += (char) d[c];
    }

    for (uint32_t c = 0x40; c < 0x5F; ++c)
    {
        if (d[c] == 0) break;

        m_name += (char)d[c];
    }

    
    for (VolFile* f : m_vol_files)
    {
        if (f->filename() == "MCG")
        {
            m_sprite_vol = f;
            break;
        }
    }
}

void MedVolEntry::create_img()
{
    if (m_sprite_vol == nullptr)
        return;


    SpriteVolEntry* tileset = nullptr;
    for (VolEntry* entry : m_sprite_vol->entries())
    {
        SpriteVolEntry* sprites = static_cast<SpriteVolEntry*>(entry);
        if (sprites->tileset_name() == m_tileset)
        {
            tileset = sprites;
            break;
        }
    }

    if (tileset == nullptr)
        return;

    uint8_t* d = (uint8_t*)m_data.constData();
    uint16_t offset = 0u;

    QImage map(m_width * SpriteVolEntry::TILE_WIDTH, m_height * SpriteVolEntry::TILE_HEIGHT, QImage::Format_ARGB32);
    map.fill(Qt::transparent);

    QPainter p(&map);

    const QVector<QImage>& tiles = tileset->tileset();

    for (uint32_t y = 0u; y < m_height; ++y)
    {
        for (uint32_t x = 0u; x < m_width; ++x)
        {
            uint8_t tile = d[m_map_data + offset++];
            if (tile >= tiles.size()) continue;
            p.drawImage(x * SpriteVolEntry::TILE_WIDTH, y * SpriteVolEntry::TILE_HEIGHT, tiles[tile]);
        }
    }

    m_map = map;
}