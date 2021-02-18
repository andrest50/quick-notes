#ifndef QNOTEBUTTON_H
#define QNOTEBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class QNoteButton : public QPushButton
{
    Q_OBJECT

public:
    explicit QNoteButton(QWidget *parent = 0);
    QNoteButton(const QString &text, QWidget *parent = nullptr);

private slots:
    void mousePressEvent(QMouseEvent *e);

signals:
    void rightClicked();
};

#endif // QNOTEBUTTON_H
