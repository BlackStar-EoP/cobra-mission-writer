#include "mainwindow.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QBoxLayout>
#include <QScrollArea>

#include "imagelabel.h"

#include "volentry.h"
#include "volfile.h"
#include "volfilelistwidget.h"
#include "volfilelistwidgetitem.h"

MainWindow::MainWindow()
{
	setAcceptDrops(true);

	QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight);

	m_vol_list_widget = new VolFileListWidget(this);
	m_vol_list_widget->setMinimumWidth(300);
	m_vol_list_widget->setMaximumWidth(300);

	m_image_label = new ImageLabel(this);

	QScrollArea* scrollArea = new QScrollArea(this);
	scrollArea->setWidget(m_image_label);
	scrollArea->setWidgetResizable(true);

	layout->addWidget(m_vol_list_widget);
	layout->addWidget(scrollArea);

	connect(m_vol_list_widget, SIGNAL(objectDoubleClicked(VolEntry*)), this, SLOT(volEntryDoubleClick(VolEntry*)));
	connect(m_vol_list_widget, SIGNAL(objectClicked(VolEntry*)), this, SLOT(volEntryClick(VolEntry*)));

	QWidget* w = new QWidget(); 
	w->setLayout(layout);
	setCentralWidget(w);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasUrls())
		e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
	if (e->mimeData()->urls().size() != 1)
		return;

	QStringList accepted_types;
	accepted_types << "vol";

	QString fname = e->mimeData()->urls()[0].toLocalFile();
	QFileInfo info(fname);
	if (info.exists())
	{
		if (accepted_types.contains(info.suffix().trimmed(), Qt::CaseInsensitive))
		{
			QFile f(fname);
			if (f.open(QIODevice::ReadOnly))
			{
				VolFile* volfile = new VolFile(info.baseName().toUpper(), f.readAll(), m_vol_files);

				m_vol_list_widget->set_vol_file(volfile);
				m_vol_files.push_back(volfile);
			}
		}
	}
}

void MainWindow::volEntryDoubleClick(VolEntry* entry)
{
	m_image_label->setImage(entry->image());
}

void MainWindow::volEntryClick(VolEntry* entry)
{
	Q_UNUSED(entry);
}
