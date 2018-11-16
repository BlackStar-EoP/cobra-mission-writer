#include "gcwidget.h"

#include <QLabel>
#include <QPushButton>

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
		m_index_label->setGeometry(WIDTH/2, 0, WIDTH, HEIGHT/2);

		m_color_label = new QLabel("", this);
		m_color_label->setGeometry(0, HEIGHT/2, WIDTH, HEIGHT/2);
		m_color_label->setAutoFillBackground(true);
	}

	void setColor(const QColor& color)
	{
		QPalette pal = palette();
		pal.setColor(backgroundRole(), color);
		m_color_label->setPalette(pal);
		m_color = color;
	}

	const QColor& getPaletteColor() const
	{
		return m_color;
	}

private:
	QLabel* m_index_label = nullptr;
	QLabel* m_color_label = nullptr;
	QColor m_color;
};

GCWidget::GCWidget(QWidget* parent)
: QWidget(parent)
{
	initUI();
}

void GCWidget::set_gc_data(const QString& filename, uint8_t* gc_data, uint32_t gc_size)
{
	delete[] m_gc_data;
	m_gc_data = gc_data;
	m_filename = filename;
	m_gc_size = gc_size;

	readGCdata();
}

void GCWidget::readGCdata()
{
	assert(m_gc_data != nullptr);
	assert(m_gc_size >= 0x30);
	/*
00 - 47 43 00 02 80 00 30 00 01 00 00 00 09 6d 00 4d
10 - 00 00 0f 00 bb 0e ff 00 00 0f 0f 0f f9 0b 77 07
20 - cc 0c 5e 09 f0 00 fa 0c fb 0d fc 0e f0 0f ff 0f
30 - 38 00 00 00 09 6d 00 00 a4 1d 00 00 90 a1 c7 6c
	*/
	if (m_gc_data[0] != 0x47 || m_gc_data[1] != 0x43)
		return;

	m_filename_label->setText(m_filename);
	m_filesize_label->setText(QString::number(m_gc_size));

	uint16_t segment_start = ((uint16_t*) m_gc_data)[3];
	uint32_t num_sub_chunks = ((uint32_t*)m_gc_data)[2];
	m_file_offset = 0x30;
	if (segment_start == 0x30)
	{
		uint16_t* palette = (uint16_t*)&m_gc_data[0x10];
		for (uint8_t i = 0; i < 16; ++i)
		{
			uint16_t color = palette[i];
			uint8_t r = ((color >> 4) & 0x0F) * 16;
			uint8_t g = (color >> 8) * 16;
			uint8_t b = (color & 0x0F) * 16;
			
			
			m_palette_widgets[i]->setColor(QColor(r, g, b));
		}
	}

	updateData();
}

void GCWidget::updateData()
{
	updateHexLabels();
	updatePixels();
	updateBinaryLabels();
}


void GCWidget::updateHexLabels()
{
	if (m_gc_data == nullptr)
		return;

	for (uint32_t i = 0; i < NUM_HEX_LABELS; ++i)
	{
		uint8_t value = m_gc_data[m_file_offset + i];
		char buffer[3];
		sprintf_s(buffer, "%02x", value);
		m_byte_hex_labels[i]->setText(buffer);
	}
}

/*
01 01 01 01 wit
00 00 00 01 licht grijs (8)
00 00 01 00 fel groen (4)
00 01 00 00 munt groen (2)
01 00 00 00 blauw (1)
*/

void GCWidget::updatePixels()
{
	uint8_t byte1 = m_gc_data[m_file_offset];
	uint8_t byte2 = m_gc_data[m_file_offset + 1];
	uint8_t byte3 = m_gc_data[m_file_offset + 2];
	uint8_t byte4 = m_gc_data[m_file_offset + 3];

	uint8_t bit = 0x80;
	for (uint8_t pixel = 0; pixel < NUM_PIXELS; ++pixel)
	{
		uint8_t palIndex = 0;
		if ((byte1 & bit) > 0) palIndex += 1;
		if ((byte2 & bit) > 0) palIndex += 2;
		if ((byte3 & bit) > 0) palIndex += 4;
		if ((byte4 & bit) > 0) palIndex += 8;

		QPalette pal = m_pixel_labels[pixel]->palette();
		pal.setColor(backgroundRole(), m_palette_widgets[palIndex]->getPaletteColor());
		m_pixel_labels[pixel]->setPalette(pal);

		bit >>= 1;
	}
}

void GCWidget::updateBinaryLabels()
{
	return;
	uint8_t byte1 = m_gc_data[m_file_offset];
	uint8_t byte2 = m_gc_data[m_file_offset + 1];
	uint8_t byte3 = m_gc_data[m_file_offset + 2];
	uint8_t byte4 = m_gc_data[m_file_offset + 3];

	for (uint32_t i = 0; i < NUM_BINARY_LABELS; ++i)
	{
		uint8_t byte = m_gc_data[m_file_offset + i];
		QString byteString;
		for (uint8_t bit = 0x80; bit != 0; bit >>= 1)
		{
			if ((byte & bit) > 0)
				byteString += "1";
			else
				byteString += "0";
		}

		m_pixel_labels[i]->setText(byteString);
	}
}

void GCWidget::initUI()
{
	QLabel* name = new QLabel("Filename:", this);
	name->setGeometry(10, 10, 120, 20);
	m_filename_label = new QLabel(this);
	m_filename_label->setGeometry(130, 10, 800, 20);

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

	for (uint32_t i = 0; i < NUM_HEX_LABELS; ++i)
	{
		const uint32_t HEX_LABEL_WIDTH = 20;
		QLabel* hexLabel = new QLabel("--", this);
		hexLabel->setFont(QFont("Courier New"));
		hexLabel->setGeometry(40 + HEX_LABEL_WIDTH * i, 100, HEX_LABEL_WIDTH, 25);
		m_byte_hex_labels[i] = hexLabel;
	}

	for (uint32_t i = 0; i < 4; ++i)
	{
		QLabel* hexLabel = m_byte_hex_labels[i];
		QPalette palette = hexLabel->palette();
		palette.setColor(foregroundRole(), Qt::red);
		hexLabel->setPalette(palette);
	}

	QPushButton* homeButton = new QPushButton("Home", this);
	homeButton->setGeometry(40, 130, 40, 20);
	connect(homeButton, SIGNAL(clicked()), this, SLOT(homeButton_clicked()));
	
	QPushButton* l4Button = new QPushButton("<<<<", this);
	l4Button->setGeometry(80, 130, 40, 20);
	connect(l4Button, SIGNAL(clicked()), this, SLOT(l4Button_clicked()));

	QPushButton* l1Button = new QPushButton("<", this);
	l1Button->setGeometry(120, 130, 40, 20);
	connect(l1Button, SIGNAL(clicked()), this, SLOT(l1Button_clicked()));

	QPushButton* r1Button = new QPushButton(">", this);
	r1Button->setGeometry(160, 130, 40, 20);
	connect(r1Button, SIGNAL(clicked()), this, SLOT(r1Button_clicked()));

	QPushButton* r4Button = new QPushButton(">>>>", this);
	r4Button->setGeometry(200, 130, 40, 20);
	connect(r4Button, SIGNAL(clicked()), this, SLOT(r4Button_clicked()));

	for (uint32_t i = 0; i < NUM_PIXELS; ++i)
	{
		const uint32_t PIXEL_LABEL_SIZE = 20;

		QLabel* pixelLabel = new QLabel("", this);
		pixelLabel->setAutoFillBackground(true);
		pixelLabel->setGeometry(40 + PIXEL_LABEL_SIZE * i, 160, PIXEL_LABEL_SIZE, PIXEL_LABEL_SIZE);
	//	pixelLabel->setStyleSheet("border: 1px solid black");
		m_pixel_labels[i] = pixelLabel;
	}

	int yBegin = 200;
	for (uint32_t i = 0; i < NUM_BINARY_LABELS; ++i)
	{
		QLabel* binaryLabel = new QLabel("00000000", this);
		binaryLabel->setGeometry(40, yBegin, 200, 20);
		yBegin += 25;
		m_binary_labels[i] = binaryLabel;
	}
}

void GCWidget::homeButton_clicked()
{
	m_file_offset = 0;
	updateData();
}

void GCWidget::l4Button_clicked()
{
	if (m_file_offset > 3)
		m_file_offset -= 4;
	else
		m_file_offset = 0;
	updateData();
}

void GCWidget::l1Button_clicked()
{
	if (m_file_offset > 0)
		m_file_offset--;

	updateData();
}

void GCWidget::r1Button_clicked()
{
	m_file_offset++;
	updateData();
}

void GCWidget::r4Button_clicked()
{
	m_file_offset += 4;
	updateData();
}