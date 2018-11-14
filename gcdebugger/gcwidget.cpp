#include "gcwidget.h"

#include <QLabel>

#include <assert.h>

class PaletteWidget : public QWidget
{
public:
	static const uint32_t WIDTH = 40;
	static const uint32_t HEIGHT = 40;

public:
	PaletteWidget(QWidget* parent, uint8_t index)
	: QWidget(parent)
	{
		m_index_label = new QLabel(QString::number(index), this);
		m_index_label->setGeometry(0, 0, WIDTH, HEIGHT/2);

		m_color_label = new QLabel("", this);
		m_color_label->setGeometry(0, HEIGHT/2, WIDTH, HEIGHT/2);
	}

	void setColor(const QColor& color)
	{
		QPalette pal = palette();
		pal.setColor(backgroundRole(), color);
		m_color_label->setPalette(pal);
	}

private:
	QLabel* m_index_label = nullptr;
	QLabel* m_color_label = nullptr;
};

GCWidget::GCWidget(QWidget* parent)
: QWidget(parent)
{
	initUI();
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

void GCWidget::initUI()
{
	QLabel* name = new QLabel("Filename:", this);
	name->setGeometry(10, 10, 120, 20);
	m_filename_label = new QLabel(this);
	m_filename_label->setGeometry(130, 10, 400, 20);

	QLabel* size = new QLabel("File size:", this);
	size->setGeometry(10, 35, 120, 20);
	m_filesize_label = new QLabel(this);
	m_filesize_label->setGeometry(130, 35, 100, 20);

	for (uint8_t i = 0; i < 16; ++i)
	{
		PaletteWidget* w = new PaletteWidget(this, i);
		m_palette_widgets[i] = w;
		w->setGeometry(10 + PaletteWidget::WIDTH * i, 60, PaletteWidget::WIDTH, PaletteWidget::HEIGHT);
	}
}