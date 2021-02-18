#include "qnotebutton.h"

QNoteButton::QNoteButton(QWidget *parent) : QPushButton(parent){

}

QNoteButton::QNoteButton(const QString &text, QWidget *parent) : QPushButton(text, parent){

}

void QNoteButton::mousePressEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton)
        emit clicked();
    else if(e->button() == Qt::RightButton)
        emit rightClicked();
}
