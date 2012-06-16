#include "Platform/View/ColorDock.h"

//#include <QAction>
//#include <QtEvents>
//#include <QFrame>
//#include <QMainWindow>
//#include <QMenu>
//#include <QPainter>
//#include <QImage>
//#include <QColor>
//#include <QDialog>
//#include <QGridLayout>
//#include <QSpinBox>
//#include <QLabel>
//#include <QPainterPath>
//#include <QPushButton>
//#include <QHBoxLayout>
//#include <QBitmap>
//#include <QtDebug>

#undef DEBUG_SIZEHINTS

#include <QPainterPath>
#include <QPainter>
#include <QMap>
#include <qdebug.h>

void render_qt_text(QPainter *painter, int w, int h, const QColor &color) {
    QPainterPath path;
    path.moveTo(-0.083695, 0.283849);
    path.cubicTo(-0.049581, 0.349613, -0.012720, 0.397969, 0.026886, 0.428917);
    path.cubicTo(0.066493, 0.459865, 0.111593, 0.477595, 0.162186, 0.482108);
    path.lineTo(0.162186, 0.500000);
    path.cubicTo(0.115929, 0.498066, 0.066565, 0.487669, 0.014094, 0.468810);
    path.cubicTo(-0.038378, 0.449952, -0.088103, 0.423839, -0.135082, 0.390474);
    path.cubicTo(-0.182061, 0.357108, -0.222608, 0.321567, -0.256722, 0.283849);
    path.cubicTo(-0.304712, 0.262250, -0.342874, 0.239362, -0.371206, 0.215184);
    path.cubicTo(-0.411969, 0.179078, -0.443625, 0.134671, -0.466175, 0.081963);
    path.cubicTo(-0.488725, 0.029255, -0.500000, -0.033043, -0.500000, -0.104932);
    path.cubicTo(-0.500000, -0.218407, -0.467042, -0.312621, -0.401127, -0.387573);
    path.cubicTo(-0.335212, -0.462524, -0.255421, -0.500000, -0.161752, -0.500000);
    path.cubicTo(-0.072998, -0.500000, 0.003903, -0.462444, 0.068951, -0.387331);
    path.cubicTo(0.133998, -0.312218, 0.166522, -0.217440, 0.166522, -0.102998);
    path.cubicTo(0.166522, -0.010155, 0.143394, 0.071325, 0.097138, 0.141441);
    path.cubicTo(0.050882, 0.211557, -0.009396, 0.259026, -0.083695, 0.283849);
    path.moveTo(-0.167823, -0.456963);
    path.cubicTo(-0.228823, -0.456963, -0.277826, -0.432624, -0.314831, -0.383946);
    path.cubicTo(-0.361665, -0.323340, -0.385082, -0.230335, -0.385082, -0.104932);
    path.cubicTo(-0.385082, 0.017569, -0.361376, 0.112025, -0.313964, 0.178433);
    path.cubicTo(-0.277248, 0.229368, -0.228534, 0.254836, -0.167823, 0.254836);
    path.cubicTo(-0.105088, 0.254836, -0.054496, 0.229368, -0.016045, 0.178433);
    path.cubicTo(0.029055, 0.117827, 0.051605, 0.028691, 0.051605, -0.088975);
    path.cubicTo(0.051605, -0.179562, 0.039318, -0.255803, 0.014744, -0.317698);
    path.cubicTo(-0.004337, -0.365409, -0.029705, -0.400548, -0.061362, -0.423114);
    path.cubicTo(-0.093018, -0.445680, -0.128505, -0.456963, -0.167823, -0.456963);
    path.moveTo(0.379011, -0.404739);
    path.lineTo(0.379011, -0.236460);
    path.lineTo(0.486123, -0.236460);
    path.lineTo(0.486123, -0.197292);
    path.lineTo(0.379011, -0.197292);
    path.lineTo(0.379011, 0.134913);
    path.cubicTo(0.379011, 0.168117, 0.383276, 0.190442, 0.391804, 0.201886);
    path.cubicTo(0.400332, 0.213330, 0.411246, 0.219052, 0.424545, 0.219052);
    path.cubicTo(0.435531, 0.219052, 0.446227, 0.215264, 0.456635, 0.207689);
    path.cubicTo(0.467042, 0.200113, 0.474993, 0.188910, 0.480486, 0.174081);
    path.lineTo(0.500000, 0.174081);
    path.cubicTo(0.488436, 0.210509, 0.471957, 0.237911, 0.450564, 0.256286);
    path.cubicTo(0.429170, 0.274662, 0.407054, 0.283849, 0.384215, 0.283849);
    path.cubicTo(0.368893, 0.283849, 0.353859, 0.279094, 0.339115, 0.269584);
    path.cubicTo(0.324371, 0.260074, 0.313530, 0.246534, 0.306592, 0.228965);
    path.cubicTo(0.299653, 0.211396, 0.296184, 0.184075, 0.296184, 0.147002);
    path.lineTo(0.296184, -0.197292);
    path.lineTo(0.223330, -0.197292);
    path.lineTo(0.223330, -0.215667);
    path.cubicTo(0.241833, -0.224049, 0.260697, -0.237992, 0.279922, -0.257495);
    path.cubicTo(0.299147, -0.276999, 0.316276, -0.300129, 0.331310, -0.326886);
    path.cubicTo(0.338826, -0.341070, 0.349523, -0.367021, 0.363400, -0.404739);
    path.lineTo(0.379011, -0.404739);
    path.moveTo(-0.535993, 0.275629);

    painter->translate(w / 2, h / 2);
    double scale = qMin(w, h) * 8 / 10.0;
    painter->scale(scale, scale);

    painter->setRenderHint(QPainter::Antialiasing);

    painter->save();
    painter->translate(.1, .1);
    painter->fillPath(path, QColor(0, 0, 0, 63));
    painter->restore();

    painter->setBrush(color);
    painter->setPen(QPen(Qt::black, 0.02, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    painter->drawPath(path);
}


QColor bgColorForName(const QString &name)
{
    if (name == "Black")
        return QColor("#D8D8D8");
    else if (name == "White")
        return QColor("#F1F1F1");
    else if (name == "Red")
        return QColor("#F1D8D8");
    else if (name == "Green")
        return QColor("#D8E4D8");
    else if (name == "Blue")
        return QColor("#D8D8F1");
    else if (name == "Yellow")
        return QColor("#F1F0D8");
    return QColor(name).light(110);
}

QColor fgColorForName(const QString &name)
{
    if (name == "Black")
        return QColor("#6C6C6C");
    else if (name == "White")
        return QColor("#F8F8F8");
    else if (name == "Red")
        return QColor("#F86C6C");
    else if (name == "Green")
        return QColor("#6CB26C");
    else if (name == "Blue")
        return QColor("#6C6CF8");
    else if (name == "Yellow")
        return QColor("#F8F76C");
    return QColor(name);
}


ColorDock::ColorDock(const QString &c, QWidget *parent)
    : QFrame(parent) , color(c)
{
    QFont font = this->font();
    font.setPointSize(8);
    setFont(font);
    szHint = QSize(-1, -1);
    minSzHint = QSize(125, 75);
}

QSize ColorDock::sizeHint() const
{
    return szHint;
}

QSize ColorDock::minimumSizeHint() const
{
    return minSzHint;
}

void ColorDock::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), bgColorForName(color));

    p.save();

    extern void render_qt_text(QPainter *, int, int, const QColor &);
    render_qt_text(&p, width(), height(), fgColorForName(color));

    p.restore();

#ifdef DEBUG_SIZEHINTS
    p.setRenderHint(QPainter::Antialiasing, false);

    QSize sz = size();
    QSize szHint = sizeHint();
    QSize minSzHint = minimumSizeHint();
    QSize maxSz = maximumSize();
    QString text = QString::fromLatin1("sz: %1x%2\nszHint: %3x%4\nminSzHint: %5x%6\n"
                                        "maxSz: %8x%9")
                    .arg(sz.width()).arg(sz.height())
                    .arg(szHint.width()).arg(szHint.height())
                    .arg(minSzHint.width()).arg(minSzHint.height())
                    .arg(maxSz.width()).arg(maxSz.height());

    QRect r = fontMetrics().boundingRect(rect(), Qt::AlignLeft|Qt::AlignTop, text);
    r.adjust(-2, -2, 1, 1);
    p.translate(4, 4);
    QColor bg = Qt::yellow;
    bg.setAlpha(120);
    p.setBrush(bg);
    p.setPen(Qt::black);
    p.drawRect(r);
    p.drawText(rect(), Qt::AlignLeft|Qt::AlignTop, text);
#endif // DEBUG_SIZEHINTS
}

static QSpinBox *createSpinBox(int value, QWidget *parent, int max = 1000)
{
    QSpinBox *result = new QSpinBox(parent);
    result->setMinimum(-1);
    result->setMaximum(max);
    result->setValue(value);
    return result;
}

void ColorDock::changeSizeHints()
{
    QDialog dialog(this);
    dialog.setWindowTitle(color);

    QVBoxLayout *topLayout = new QVBoxLayout(&dialog);

    QGridLayout *inputLayout = new QGridLayout();
    topLayout->addLayout(inputLayout);

    inputLayout->addWidget(new QLabel(tr("Size Hint:"), &dialog), 0, 0);
    inputLayout->addWidget(new QLabel(tr("Min Size Hint:"), &dialog), 1, 0);
    inputLayout->addWidget(new QLabel(tr("Max Size:"), &dialog), 2, 0);
    inputLayout->addWidget(new QLabel(tr("Dockwgt Max Size:"), &dialog), 3, 0);

    QSpinBox *szHintW = createSpinBox(szHint.width(), &dialog);
    inputLayout->addWidget(szHintW, 0, 1);
    QSpinBox *szHintH = createSpinBox(szHint.height(), &dialog);
    inputLayout->addWidget(szHintH, 0, 2);

    QSpinBox *minSzHintW = createSpinBox(minSzHint.width(), &dialog);
    inputLayout->addWidget(minSzHintW, 1, 1);
    QSpinBox *minSzHintH = createSpinBox(minSzHint.height(), &dialog);
    inputLayout->addWidget(minSzHintH, 1, 2);

    QSize maxSz = maximumSize();
    QSpinBox *maxSzW = createSpinBox(maxSz.width(), &dialog, QWIDGETSIZE_MAX);
    inputLayout->addWidget(maxSzW, 2, 1);
    QSpinBox *maxSzH = createSpinBox(maxSz.height(), &dialog, QWIDGETSIZE_MAX);
    inputLayout->addWidget(maxSzH, 2, 2);

    QSize dwMaxSz = parentWidget()->maximumSize();
    QSpinBox *dwMaxSzW = createSpinBox(dwMaxSz.width(), &dialog, QWIDGETSIZE_MAX);
    inputLayout->addWidget(dwMaxSzW, 3, 1);
    QSpinBox *dwMaxSzH = createSpinBox(dwMaxSz.height(), &dialog, QWIDGETSIZE_MAX);
    inputLayout->addWidget(dwMaxSzH, 3, 2);

    inputLayout->setColumnStretch(1, 1);
    inputLayout->setColumnStretch(2, 1);

    topLayout->addStretch();

    QHBoxLayout *buttonBox = new QHBoxLayout();
    topLayout->addLayout(buttonBox);

    QPushButton *okButton = new QPushButton(tr("Ok"), &dialog);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), &dialog);
    connect(okButton, SIGNAL(clicked()), &dialog, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), &dialog, SLOT(reject()));
    buttonBox->addStretch();
    buttonBox->addWidget(cancelButton);
    buttonBox->addWidget(okButton);


    if (!dialog.exec())
        return;

    szHint = QSize(szHintW->value(), szHintH->value());
    minSzHint = QSize(minSzHintW->value(), minSzHintH->value());
    maxSz = QSize(maxSzW->value(), maxSzH->value());
    setMaximumSize(maxSz);
    dwMaxSz = QSize(dwMaxSzW->value(), dwMaxSzH->value());
    parentWidget()->setMaximumSize(dwMaxSz);
    updateGeometry();
    update();
}

void ColorDock::setCustomSizeHint(const QSize &size)
{
    szHint = size;
    updateGeometry();
}
