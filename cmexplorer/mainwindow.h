#pragma once

#include <QMainWindow>
#include <stdint.h>

class ImageLabel;

class VolFileListWidget;
class VolEntry;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();

	void dragEnterEvent(QDragEnterEvent *e) override;
	void dropEvent(QDropEvent *e) override;

private slots:
	void volEntryDoubleClick(VolEntry* entry);
	void volEntryClick(VolEntry* entry);

private:
	VolFileListWidget* m_vol_list_widget = nullptr;
	ImageLabel* m_image_label = nullptr;
};