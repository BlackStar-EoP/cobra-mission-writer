#include "mainwindow.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QDebug>
#include <QFileInfo>
#include <QFile>

#include "gcwidget.h"

MainWindow::MainWindow()
{
	setAcceptDrops(true);

	m_GC_widget = new GCWidget(this);
	setCentralWidget(m_GC_widget);
	m_GC_widget->show();
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
	accepted_types << "gc";

	QString fname = e->mimeData()->urls()[0].toLocalFile();
	QFileInfo info(fname);
	if (info.exists())
	{
		if (accepted_types.contains(info.suffix().trimmed(), Qt::CaseInsensitive))
		{
			QFile f(fname);
			if (f.open(QIODevice::ReadOnly))
			{
				qint64 size = f.size();
				if (size > 0)
				{
					uint8_t* gc_data = new uint8_t[size];
					memcpy(gc_data, f.readAll().data(), size);
					f.close();
					m_GC_widget->set_gc_data(fname, gc_data);
				}

			}
		}
	}
}