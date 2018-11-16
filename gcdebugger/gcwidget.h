#include <QWidget>

class QLabel;
class PaletteWidget;

class GCWidget : public QWidget
{
	Q_OBJECT
public:
	GCWidget(QWidget* parent);
	void set_gc_data(const QString& filename, uint8_t* gc_data, uint32_t gc_size);

private:
	void readGCdata();
	void updateData();
	void updateHexLabels();
	void updatePixels();
	void updateBinaryLabels();
	void initUI();

private slots:
	void homeButton_clicked();
	void l4Button_clicked();
	void l1Button_clicked();
	void r1Button_clicked();
	void r4Button_clicked();

private:
	QString m_filename;
	uint8_t* m_gc_data = nullptr;
	uint32_t m_gc_size = 0;
	uint32_t m_file_offset = 0;
	QLabel* m_filename_label = nullptr;
	QLabel* m_filesize_label = nullptr;

	PaletteWidget* m_palette_widgets[16];

	static const uint32_t NUM_HEX_LABELS = 32;
	QLabel* m_byte_hex_labels[NUM_HEX_LABELS];

	static const uint32_t NUM_PIXELS = 8;
	QLabel* m_pixel_labels[NUM_PIXELS];

	static const uint32_t NUM_BINARY_LABELS = 4;
	QLabel* m_binary_labels[NUM_BINARY_LABELS];
};