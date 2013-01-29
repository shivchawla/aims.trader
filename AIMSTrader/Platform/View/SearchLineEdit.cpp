#include "Platform/View/SearchLineEdit.h"
#include <QToolButton>
#include <QStyle>
#include <QKeyEvent>

SearchLineEdit::SearchLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    // Create the search button and set its icon, cursor, and stylesheet
//    this->mSearchButton = new QToolButton(this);
//    this->mSearchButton->setFixedSize(18, 18);
//    this->mSearchButton->setCursor(Qt::ArrowCursor);
//    //this->mSearchButton->setStyleSheet(this->buttonStyleSheetForCurrentState());

    // Update the search button when the text changes
    QObject::connect(this, SIGNAL(textChanged(QString)), SLOT(updateSearchButton(QString)));

    // Some stylesheet and size corrections for the text box
    this->setPlaceholderText(tr("Search"));
//    this->setStyleSheet(this->styleSheetForCurrentState());

//    int frameWidth = this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
//    QSize minSizeHint = this->minimumSizeHint();
//    this->setMinimumSize(qMax(minSizeHint.width(), this->mSearchButton->sizeHint().width() + frameWidth * 2 + 2),
//                         qMax(minSizeHint.height(), this->mSearchButton->sizeHint().height() + frameWidth * 2 + 2));
//    this->setSizePolicy(QSizePolicy::Maximum, this->sizePolicy().verticalPolicy());
}

void SearchLineEdit::resizeEvent(QResizeEvent *event)
{
    QLineEdit::resizeEvent(event);
//    Q_UNUSED(event);
//    QSize size = this->mSearchButton->sizeHint();
//    int frameWidth = this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
//    this->mSearchButton->move(this->rect().right() - frameWidth - size.width() - 2, (this->rect().bottom() + 2 - size.height()) / 2);
}

void SearchLineEdit::updateSearchButton(const QString &text)
{
    if (!text.isEmpty())
    {
        // We have some text in the box - set the button to clear the text
        //QObject::connect(this->mSearchButton, SIGNAL(clicked()), SLOT(clear()));
    }
    else
    {
        // The text box is empty - make the icon do nothing when clicked
        //QObject::disconnect(this->mSearchButton, SIGNAL(clicked()), this, SLOT(clear()));
    }

    //this->setStyleSheet(this->styleSheetForCurrentState());
    //this->mSearchButton->setStyleSheet(this->buttonStyleSheetForCurrentState());
}

QString SearchLineEdit::styleSheetForCurrentState() const
{
    int frameWidth = this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

    QString style;
    style += "QLineEdit {";
    if (this->text().isEmpty())
    {
        style += "font-family: 'MS Sans Serif';";
        style += "font-style: italic;";
    }

    style += "padding-left: 3px;";
    style += QString("padding-right: %1px;").arg(this->mSearchButton->sizeHint().width() + frameWidth + 1);
    style += "border-width: 3px;";
    style += "border-image: url(:/images/esf-border.png) 3 3 3 3 stretch;";
    style += "background-color: rgba(255, 255, 255, 204);";
    style += "}";
    style += "QLineEdit:hover, QLineEdit:focus {";
    style += "background-color: rgba(255, 255, 255, 255);";
    style += "}";
    return style;
}

QString SearchLineEdit::buttonStyleSheetForCurrentState() const
{
    QString style;
    style += "QToolButton {";
    style += "border: none; margin: 0; padding: 0;";
    style += QString("background-image: url(:/images/esf-%1.png);").arg(this->text().isEmpty() ? "search" : "clear");
    style += "}";

    if (!this->text().isEmpty())
    {
        style += "QToolButton:hover { background-image: url(:/images/esf-clear-hover.png); }";
        style += "QToolButton:pressed { background-image: url(:/images/esf-clear-active.png); }";
    }

    return style;
}

void SearchLineEdit::keyPressEvent ( QKeyEvent * event )
{
    if(event->key() == Qt::Key_Escape)
    {
        //emit returnPressed();
        emit textChanged("");
    }

    QLineEdit::keyPressEvent(event);
}
