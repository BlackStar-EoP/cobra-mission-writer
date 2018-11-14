#pragma once

#include <QMainWindow>
#include <stdint.h>

class MainWindow : public QMainWindow
{
public:
	MainWindow();

	void dragEnterEvent(QDragEnterEvent *e) override;
	void dropEvent(QDropEvent *e) override;

private:
	uint8_t* m_gc_data = nullptr;
};