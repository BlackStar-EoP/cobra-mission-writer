#include <QWidget>

class GCWidget : public QWidget
{
public:
	GCWidget(QWidget* parent);
	void set_gc_data(const QString& filename, uint8_t* gc_data);

private:
	void readGCdata();

private:
	QString m_filename;
	uint8_t* m_gc_data = nullptr;
};