#pragma once

#include <QByteArray>
#include <QVector>

class VolEntry;

class VolFile
{
public:
    enum EVolType
    {
        GC,
        MAPS,
        SPRITES,
        UNKNOWN,
    };

public:
    VolFile() = default;
    VolFile(const QString& filename, QByteArray data);
    QVector<VolEntry*>& entries();

private:
    void parse_entries();

private:
    QString m_filename;
    QByteArray m_data;
    QVector<VolEntry*> m_entries;
};