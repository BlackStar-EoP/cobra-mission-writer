#include "gcwidget.h"

#include <assert.h>

GCWidget::GCWidget(QWidget* parent)
: QWidget(parent)
{
}

void GCWidget::set_gc_data(const QString& filename, uint8_t* gc_data)
{
	delete[] m_gc_data;
	m_gc_data = gc_data;
	m_filename = filename;

	readGCdata();
}

void GCWidget::readGCdata()
{
	assert(m_gc_data != nullptr);
}
