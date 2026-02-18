#pragma once

#include "volentry.h"

class SpriteVolEntry : public VolEntry
{
public:
    static const uint32_t TILE_WIDTH = 32u;
    static const uint32_t TILE_HEIGHT = 32u;

public:
    SpriteVolEntry(uint32_t index, const QString& tileset_name, QByteArray data);

    QImage image() override;
    QString to_string() override;
    void next() override;
    void prev() override;
    void check(bool checked) override;

    const QVector<QImage>& tileset() const;
    const QString& tileset_name() const;

private:
    void parse_tiles();
    void create_tile_img();

private:
    QImage m_tileset_img;
    QVector<QImage> m_tileset;
    QString m_tileset_name;
};