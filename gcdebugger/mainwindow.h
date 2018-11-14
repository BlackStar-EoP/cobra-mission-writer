#pragma once

#include <QMainWindow>
#include <stdint.h>

class GCWidget;

class MainWindow : public QMainWindow
{
public:
	MainWindow();

	void dragEnterEvent(QDragEnterEvent *e) override;
	void dropEvent(QDropEvent *e) override;

private:
	GCWidget* m_GC_widget = nullptr;
};