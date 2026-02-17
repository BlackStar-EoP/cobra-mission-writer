#include "spritevolentry.h"

#include <QPainter>

typedef uint8_t byte;

class Color
{
public:
	Color()
		: m_r(0)
		, m_g(0)
		, m_b(0)
		, m_a(0)
	{
	}
	Color(byte r, byte g, byte b)
		: m_r(r)
		, m_g(g)
		, m_b(b)
		, m_a(0xFF)
	{

	}

	byte m_r;
	byte m_g;
	byte m_b;
	byte m_a;
};

Color get_color(byte c)
{
	switch (c)
	{
	case 0x00: 	return Color(0, 0, 0);
	case 0x66: return Color(195, 195, 0);  // - 195 195 0 
	case 0xEE: return Color(243, 243, 0);  // - 243 243 0 
	case 0xFF: return Color(243, 243, 243); // - 243 243 243
	case 0x77: return Color(113, 113, 113); // - 113 113 113
	case 0x33: return Color(243, 211, 162); // - 243 211 162
	case 0x22: return Color(195, 0, 0); // - 195 0 0
	case 0xAA: return Color(243, 0, 0); // - 243 0 0
	case 0x88: return Color(195, 195, 195); // - 195 195 195
	case 0x44: return Color(0, 178, 0);
	case 0xCC: return Color(0, 243, 0);
	case 0x11: return Color(0, 0, 195); //- 0, 0, 195
	case 0x99: return Color(0, 0, 243); //- 0, 0, 243
	case 0x55: return Color(0, 195, 195);
	case 0xDD: return Color(0, 243, 243);
	case 0xBB: return Color(0, 243, 243);
		//case 0x    return Color(0, 243, 243);
		//case 0x40: return Color(255, 0, 255);
		//case 0xA0: return Color(0, 243, 243);
		//case 0x04: return Color(255, 128, 64);

	}

	return Color(0xFF, 0x00, 0xFF);
}


SpriteVolEntry::SpriteVolEntry(uint32_t index, const QString& tileset_name, QByteArray data)
: VolEntry(index, data)
, m_tileset_name(tileset_name)
{
	parse_tiles();
	create_tile_img();
}

QImage SpriteVolEntry::image()
{
    return m_tileset_img;
}

QString SpriteVolEntry::to_string()
{
    return QString::number(index()) + QString(" - ") + m_tileset_name;
}

const QVector<QImage>& SpriteVolEntry::tileset() const
{
	return m_tileset;
}

void SpriteVolEntry::parse_tiles()
{
	uint32_t num_tiles = m_data.size() / TILE_WIDTH / TILE_HEIGHT;

	uint8_t* d = (uint8_t*) m_data.constData();
	for (uint32_t tile = 0u; tile < num_tiles; ++tile)
	{
		uint8_t* tile_data = d + (tile * (TILE_WIDTH * TILE_HEIGHT));
		uint32_t offset = 0u;
		QImage tile_img = QImage(TILE_WIDTH, TILE_HEIGHT, QImage::Format_ARGB32);
		tile_img.fill(Qt::transparent);
		QPainter p(&tile_img);
		for (uint32_t y = 0u; y < TILE_HEIGHT; ++y)
		{
			for (uint32_t x = 0u; x < TILE_WIDTH; ++x)
			{
				uint8_t pixel = tile_data[offset++];
				Color c = get_color(pixel);
				p.setPen(QColor(c.m_r, c.m_g, c.m_b, c.m_a));
				p.drawLine(x, y, x, y);
			}
		}
		m_tileset.push_back(tile_img);
	}
}

void SpriteVolEntry::create_tile_img()
{
	uint32_t tile_grid = ceil(sqrtf(m_tileset.size()));

	uint32_t tile_idx = 0u;
	QImage tile_atlas = QImage(tile_grid * TILE_WIDTH, tile_grid * TILE_HEIGHT, QImage::Format_ARGB32);
	tile_atlas.fill(Qt::transparent);

	QPainter p(&tile_atlas);
	for (uint32_t y = 0; y < tile_grid; ++y)
	{
		for (uint32_t x = 0; x < tile_grid; ++x)
		{
			if (tile_idx >= m_tileset.size()) goto END_REACHED;

			QImage t = m_tileset[tile_idx];
			p.drawImage(x * TILE_WIDTH, y * TILE_HEIGHT, t);
			tile_idx++;
		}
	}

	END_REACHED:
	m_tileset_img = tile_atlas;
}

const QString& SpriteVolEntry::tileset_name() const
{
	return m_tileset_name;
}