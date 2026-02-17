#pragma once

#include <QFont>
#include <QListWidget>
#include <QMap>

class VolEntry;
class VolFile;

class VolFileListWidget : public QListWidget
{
    Q_OBJECT

public:
    VolFileListWidget(QWidget* parent);
    ~VolFileListWidget();
    void set_vol_file(VolFile* vol_file);

public:
    void refreshList();

signals:
    void objectClicked(VolEntry* entry);
    void objectDoubleClicked(VolEntry* entry);

private slots:
    void objectDoubleClick(QListWidgetItem* item);
    void objectClick(QListWidgetItem* item);

private:
    VolFile* m_vol_file = nullptr;
};