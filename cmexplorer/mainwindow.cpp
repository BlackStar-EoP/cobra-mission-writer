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
#include "gcvolentry.h"
#include "cm98mapvolentry.h"

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
	accepted_types << "vol" << "gsc" << "map" << "att";

	QString fname = e->mimeData()->urls()[0].toLocalFile();
	QFileInfo info(fname);
	if (info.exists())
	{
		QString suffix = info.suffix().trimmed().toUpper();
		if (accepted_types.contains(suffix, Qt::CaseInsensitive))
		{
			QFile f(fname);
			if (f.open(QIODevice::ReadOnly))
			{
				if (suffix == "VOL")
				{
					VolFile* volfile = new VolFile(info.baseName().toUpper(), f.readAll(), m_vol_files);
					m_vol_list_widget->set_vol_file(volfile);
					m_vol_files.push_back(volfile);
				}
				else if (suffix == "GSC")
				{
					const QString cm98gsc_name = "CM98GSC";
					VolFile* cm98gsc = nullptr;
					for (VolFile* file : m_vol_files)
					{
						if (file->filename() == cm98gsc_name)
						{
							cm98gsc = file;
							break;
						}
					}

					if (cm98gsc == nullptr)
					{
						cm98gsc = new VolFile(cm98gsc_name, m_vol_files);
						m_vol_files.push_back(cm98gsc);
					}

					cm98gsc->add_entry(new GCVolEntry(cm98gsc->entries().size(), info.baseName(), f.readAll()));
					m_vol_list_widget->set_vol_file(cm98gsc);
				}
				else if (suffix == "MAP")
				{
					const QString cm98map_name = "CM98MAP";
					VolFile* cm98map = nullptr;
					for (VolFile* file : m_vol_files)
					{
						if (file->filename() == cm98map_name)
						{
							cm98map = file;
							break;
						}
					}

					if (cm98map == nullptr)
					{
						cm98map = new VolFile(cm98map_name, m_vol_files);
						m_vol_files.push_back(cm98map);
					}

					const QString cm98gsc_name = "CM98GSC";
					VolFile* cm98gsc = nullptr;
					for (VolFile* file : m_vol_files)
					{
						if (file->filename() == cm98gsc_name)
						{
							cm98gsc = file;
							break;
						}
					}

					cm98map->add_entry(new CM98MapVolEntry(cm98map->entries().size(), info.baseName().toUpper(), f.readAll(), cm98gsc));
					m_vol_list_widget->set_vol_file(cm98map);


				}
				else if (suffix == "ATT")
				{
					const QString cm98map_name = "CM98MAP";
					VolFile* cm98map = nullptr;
					for (VolFile* file : m_vol_files)
					{
						if (file->filename() == cm98map_name)
						{
							cm98map = file;
							break;
						}
					}

					if (cm98map != nullptr)
					{
						for (VolEntry* entry : cm98map->entries())
						{
							CM98MapVolEntry* map_entry = static_cast<CM98MapVolEntry*>(entry);
							if (entry->to_string() == info.baseName().toUpper())
							{
								map_entry->set_trigger_data(f.readAll());
							}
						}
					}
				}
				f.close();
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
