#pragma once

#include <QListWidgetItem>
#include <QFont>

class VolEntry;
class QLabel;

class VolFileListWidgetItem : public QFrame
{
    Q_OBJECT

public:
    static const size_t ITEM_HEIGHT = 30;

    VolFileListWidgetItem(QWidget* parent, VolEntry* entry);
    VolEntry* entry() const;

private:
    QLabel* m_object_label = nullptr;
    VolEntry* m_vol_entry = nullptr;
};