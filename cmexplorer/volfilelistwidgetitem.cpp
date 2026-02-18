#include "volfilelistwidgetitem.h"

#include "volentry.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QStyle>
#include <QCheckBox>

VolFileListWidgetItem::VolFileListWidgetItem(QWidget* parent, VolEntry* entry)
: QFrame(parent)
, m_vol_entry(entry)
{
    m_object_label = new QLabel(m_vol_entry->to_string());

    QBoxLayout* layout = new QHBoxLayout(nullptr);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->addWidget(m_object_label, 0, Qt::AlignLeft);

    QPushButton* nextButton = new QPushButton("+");
    QPushButton* prevButton = new QPushButton("-");
    QCheckBox* checkBox = new QCheckBox();

    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextClicked()));
    connect(prevButton, SIGNAL(clicked()), this, SLOT(prevClicked()));
    connect(checkBox, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(check(Qt::CheckState)));

    layout->addWidget(nextButton, 0, Qt::AlignRight);
    layout->addWidget(prevButton, 0, Qt::AlignRight);
    layout->addWidget(checkBox, 0, Qt::AlignRight);

    setLayout(layout);
}

VolEntry* VolFileListWidgetItem::entry() const
{
    return m_vol_entry;
}

void VolFileListWidgetItem::nextClicked()
{
    m_vol_entry->next();
    emit contentChanged(m_vol_entry);
}

void VolFileListWidgetItem::prevClicked()
{
    m_vol_entry->prev();
    emit contentChanged(m_vol_entry);
}

void VolFileListWidgetItem::check(Qt::CheckState state)
{
    switch (state)
    {
        case Qt::Unchecked:
            m_vol_entry->check(false);
            break;

        case Qt::PartiallyChecked:
        case Qt::Checked:
            m_vol_entry->check(true);
            break;
    }

    emit contentChanged(m_vol_entry);
}