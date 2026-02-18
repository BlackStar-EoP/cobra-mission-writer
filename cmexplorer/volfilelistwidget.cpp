#include "volfilelistwidget.h"

#include "volfilelistwidgetitem.h"

#include "volfile.h"
#include "volentry.h"

VolFileListWidget::VolFileListWidget(QWidget* parent)
: QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(objectDoubleClick(QListWidgetItem*)));
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(objectClick(QListWidgetItem*)));

    refreshList();
}

VolFileListWidget::~VolFileListWidget()
{
}

void VolFileListWidget::set_vol_file(VolFile* vol_file)
{
    m_vol_file = vol_file;
    refreshList();
}

void VolFileListWidget::refreshList()
{
    clear();
    if (m_vol_file != nullptr)
    {
        for (VolEntry* entry : m_vol_file->entries())
        {
            VolFileListWidgetItem* item = new VolFileListWidgetItem(nullptr, entry);

            connect(item, SIGNAL(contentChanged(VolEntry*)), this, SLOT(contentChanged(VolEntry*)));

            QListWidgetItem* listitem = new QListWidgetItem();
            listitem->setSizeHint(QSize(0, VolFileListWidgetItem::ITEM_HEIGHT));
            addItem(listitem);
            setItemWidget(listitem, item);
        }
    }
}

void VolFileListWidget::contentChanged(VolEntry* entry)
{
    emit objectDoubleClicked(entry);
}

void VolFileListWidget::objectDoubleClick(QListWidgetItem* item)
{
    VolFileListWidgetItem* vol_entry_item = static_cast<VolFileListWidgetItem*>(itemWidget(item));
    emit objectDoubleClicked(vol_entry_item->entry());
}

void VolFileListWidget::objectClick(QListWidgetItem* item)
{
    VolFileListWidgetItem* vol_entry_item = static_cast<VolFileListWidgetItem*>(itemWidget(item));
    emit objectClicked(vol_entry_item->entry());
}
