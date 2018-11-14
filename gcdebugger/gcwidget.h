#include <QWidget>

class QLabel;
class PaletteWidget;

class GCWidget : public QWidget
{
public:
	GCWidget(QWidget* parent);
	void set_gc_data(const QString& filename, uint8_t* gc_data);

private:
	void readGCdata();
	void initUI();

private:
	QString m_filename;
	uint8_t* m_gc_data = nullptr;

	QLabel* m_filename_label = nullptr;
	QLabel* m_filesize_label = nullptr;

	PaletteWidget* m_palette_widgets[16];
};