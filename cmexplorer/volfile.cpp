#include "volfile.h"

#include "gcvolentry.h"
#include "spritevolentry.h"
#include "medvolentry.h"

#include <QMap>

static QMap<QString, VolFile::EVolType> volFileMap =
{
    {"CODE", VolFile::UNKNOWN },
    {"CUT1", VolFile::GC },
    {"CUT2", VolFile::GC },
    {"CUT3", VolFile::GC },
    {"CUTA", VolFile::GC },
    {"DAT", VolFile::UNKNOWN },
    {"ECG", VolFile::UNKNOWN },
    {"EMI", VolFile::UNKNOWN },
    {"ENM", VolFile::GC },
    {"ENMA", VolFile::GC },
    {"ICP", VolFile::UNKNOWN },
    {"MAP", VolFile::GC },
    {"MCG", VolFile::SPRITES },
    {"MED", VolFile::MAPS },
    {"OPENING", VolFile::GC },
    {"PCM1", VolFile::UNKNOWN },
    {"PCM2", VolFile::UNKNOWN },
    {"PCM3", VolFile::UNKNOWN },
    {"PCM4", VolFile::UNKNOWN },
    {"PCM5", VolFile::UNKNOWN },
    {"PCM6", VolFile::UNKNOWN },
    {"PIC1", VolFile::GC },
    {"PIC2", VolFile::GC },
    {"PIC3", VolFile::GC },
    {"PICA", VolFile::GC },
};

static QMap<uint32_t, QString> tilesets =
{
    { 0u, "Character" },
    { 1u, "Weapon" },
    { 2u, "CASTLE" },
    { 3u, "AREA1" },
    { 4u, "AREA2" },
    { 5u, "AREA3" },
    { 6u, "AREA4" },
    { 7u, "AREA5" },
    { 8u, "HOUSE" },
    { 9u, "BAR" },
    { 10u, "CORP" },
    { 11u, "BRICK" },
    { 12u, "LAB" },
    { 13u, "SEASIDE" },
};

VolFile::VolFile(const QString& filename, QByteArray data, QVector<VolFile*>& vol_files)
: m_filename(filename)
, m_data(data)
, m_vol_files(vol_files)
{
    parse_entries();
}

void VolFile::parse_entries()
{
    uint32_t* header = (uint32_t*)m_data.data();
    uint32_t header_size = *header;
    uint32_t num_entries = header_size / sizeof(uint32_t);
    for (uint32_t i = 0u; i < (num_entries - 1u); ++i)
    {
        uint32_t start = header[i];
        uint32_t end = header[i + 1u];

        uint32_t size = end - start;
        if (size == 0 || end == 0)
            continue;

        QByteArray entry = m_data.mid(start, size);
        VolFile::EVolType vol_type = volFileMap[m_filename];

        switch (vol_type)
        {
            case GC:
                m_entries.push_back(new GCVolEntry(i, entry));
                break;
            case MAPS:
                m_entries.push_back(new MedVolEntry(i, entry, m_vol_files));
                break;
            case SPRITES:
                m_entries.push_back(new SpriteVolEntry(i, tilesets[i], entry));
                break;
            case UNKNOWN:
                printf("");
                break;
        }
    }
}

QVector<VolEntry*>& VolFile::entries()
{
    return m_entries;
}

QString VolFile::filename() const
{
    return m_filename;
}