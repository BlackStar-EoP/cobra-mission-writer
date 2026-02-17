#include "volfilelistwidgetitem.h"

#include "volentry.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QStyle>

VolFileListWidgetItem::VolFileListWidgetItem(QWidget* parent, VolEntry* entry)
: QFrame(parent)
, m_vol_entry(entry)
{
    m_object_label = new QLabel(m_vol_entry->to_string());

    QBoxLayout* layout = new QHBoxLayout(nullptr);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->addWidget(m_object_label, 0, Qt::AlignLeft);
    setLayout(layout);
}

VolEntry* VolFileListWidgetItem::entry() const
{
    return m_vol_entry;
}