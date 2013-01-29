#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QLineEdit>

class QToolButton;

class SearchLineEdit : public QLineEdit
{
    Q_OBJECT
    public:
        explicit SearchLineEdit(QWidget *parent = NULL);

    protected:
        void resizeEvent(QResizeEvent *event);
        void keyPressEvent (QKeyEvent * event);

    private slots:
        void updateSearchButton(const QString &text);

    private:
        QString styleSheetForCurrentState() const;
        QString buttonStyleSheetForCurrentState() const;

        QToolButton *mSearchButton;
};

#endif // SEARCHLINEEDIT_H
