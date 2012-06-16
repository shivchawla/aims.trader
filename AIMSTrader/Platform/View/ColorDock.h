#ifndef COLORDOCK_H
#define COLORDOCK_H

#include <QFrame>
#include <QAction>
#include <QtEvents>
#include <QFrame>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QDialog>
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPainterPath>
#include <QPushButton>
#include <QHBoxLayout>
#include <QBitmap>
#include <QtDebug>

class ColorDock : public QFrame
{
    Q_OBJECT
public:
    ColorDock(const QString &c, QWidget *parent);

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    void setCustomSizeHint(const QSize &size);

public slots:
    void changeSizeHints();

protected:
    void paintEvent(QPaintEvent *);
    QString color;
    QSize szHint, minSzHint;
};


#endif // COLORDOCK_H
