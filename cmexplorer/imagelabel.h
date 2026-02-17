#pragma once

#include <QLabel>
#include <QImage>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFileDialog>
#include <QAction>

class ImageLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ImageLabel(QWidget* parent = nullptr)
        : QLabel(parent)
    {
    }

    void setImage(const QImage& img)
    {
        image = img;
        setPixmap(QPixmap::fromImage(image));
    }

protected:
    void contextMenuEvent(QContextMenuEvent* event) override
    {
        if (image.isNull())
            return;

        QMenu menu(this);
        QAction* saveAction = menu.addAction("Save as PNG...");

        QAction* selected = menu.exec(event->globalPos());
        if (selected == saveAction)
        {
            QString fileName = QFileDialog::getSaveFileName(
                this,
                "Save Image",
                "image.png",
                "PNG Images (*.png)"
            );

            if (!fileName.isEmpty())
            {
                image.save(fileName, "PNG");
            }
        }
    }

private:
    QImage image;
};