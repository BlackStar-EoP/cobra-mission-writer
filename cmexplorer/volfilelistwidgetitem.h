#pragma once

#include <QListWidgetItem>
#include <QFont>
#include <QCheckBox>

class VolEntry;
class QLabel;

class VolFileListWidgetItem : public QFrame
{
    Q_OBJECT

public:
    static const size_t ITEM_HEIGHT = 30;

    VolFileListWidgetItem(QWidget* parent, VolEntry* entry);
    VolEntry* entry() const;

signals:
    void contentChanged(VolEntry*);

private slots:
    void nextClicked();
    void prevClicked();
    void check(Qt::CheckState state);

private:
    QLabel* m_object_label = nullptr;
    VolEntry* m_vol_entry = nullptr;
};