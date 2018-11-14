#include "mainwindow.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QDebug>
#include <QFileInfo>
#include <QFile>

MainWindow::MainWindow()
{
	setAcceptDrops(true);

	QWidget* w = new QWidget(this);
	setCentralWidget(w);
	w->show();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasUrls()) {
		e->acceptProposedAction();
	}
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
				if (m_gc_data != nullptr)
				{
					delete[] m_gc_data;
					m_gc_data = nullptr;
				}

				qint64 size = f.size();
				if (size > 0)
					m_gc_data = new uint8_t[size];

				memcpy(m_gc_data, f.readAll().data(), size);
				f.close();
			}
		}
	}
}