#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>
#include <QList>
#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);

    widget = new QWidget;
    setCentralWidget(widget);

    fileLabelMsg = QString("Current Selected File: [New File]");
    fileLabel = new QLabel(fileLabelMsg);
    fileLabel->setAlignment(Qt::AlignHCenter);

    QString notesLabelMsg = QString("Type a note into the textbox and it will appear here.");
    notesLabel = new QLabel(notesLabelMsg);
    notesLabel->setAlignment(Qt::AlignHCenter);

    container = new QWidget();
    btnsLayout = new QVBoxLayout(container);
    btnsLayout->addSpacing(0);
    btnsLayout->setContentsMargins(0, 0, 0 , 0);
    btnsLayout->addStretch(0);
    btnsLayout->setDirection(QBoxLayout::BottomToTop);

    notesBox = new QScrollArea(widget);
    notesBox->setWidget(container);
    notesBox->setWidgetResizable(true);

    inputNote = new QLineEdit(widget);
    connect(inputNote, SIGNAL (returnPressed()), this, SLOT (updateFileContent()));

    layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(fileLabel);
    layout->addWidget(notesBox);
    layout->addWidget(inputNote);
    widget->setLayout(layout);

    createActions();
    createMenus();

    setWindowTitle(tr("Quick Notes"));
    setMinimumSize(160, 160);
    resize(480, 320);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::deleteNote(){
    auto itr = std::find_if(notesBtns.begin(), notesBtns.end(), [this](QNoteButton* notesBtns)
        {return notesBtns->text() == noteClicked->text();});
    if(itr != notesBtns.end()){
        notesBtns.erase(itr);
    }
    delete noteClicked;
}

void MainWindow::noteOptions(){
    qDebug() << "Right click";

    noteClicked = qobject_cast<QNoteButton*>(sender());

    deleteNoteAct = new QAction(tr("&Delete"));
    deleteNoteAct->setStatusTip(tr("Delete note"));
    connect(deleteNoteAct, &QAction::triggered, this, &MainWindow::deleteNote);

    noteMenu = new QMenu(tr("Note Menu"), this);
    noteMenu->addAction(deleteNoteAct);
    noteMenu->exec(mapTo(0, QCursor::pos()));
}

void MainWindow::updateFileContent(){
    notes += inputNote->text();
    notes += "\n";
    //notesLabel->setText(notes.join(" "));
    QNoteButton* newNote = new QNoteButton(inputNote->text(), container);
    connect(newNote, SIGNAL (clicked()), this, SLOT (updateInputNoteText()));
    connect(newNote, SIGNAL (rightClicked()), this, SLOT (noteOptions()));
    //newNote->setStyleSheet("border: 0px;");
    notesBtns.push_back(newNote);
    btnsLayout->addWidget(newNote);
    if(currentFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        QTextStream stream(&currentFile);
        stream << inputNote->text() << Qt::endl;
    }
    inputNote->clear();
    currentFile.close();
}

void MainWindow::updateInputNoteText(){
    QNoteButton* button = qobject_cast<QNoteButton*>(sender());
    inputNote->setText(button->text());
    QTimer::singleShot(0, inputNote, SLOT(setFocus()));
}

void MainWindow::openFile(){
    QFileDialog fileDialog(this, Qt::Dialog);
    currentFileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "/Andres/Text-Files", tr("Text Files (*.txt)"));
    currentFile.setFileName(currentFileName);
    fileLabelMsg = QString("Current Selected File: %1").arg(currentFileName);
    fileLabel->setText(fileLabelMsg);

    if (!currentFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&currentFile);
    notes = QStringList("");

    if (btnsLayout != NULL){
        QLayoutItem* item;
        while ((item = btnsLayout->takeAt(0)) != NULL){
            delete item->widget();
            delete item;
        }
    }
    notesBtns.clear();

    while(!currentFile.atEnd()){
        QString line = currentFile.readLine();
        if(line != "\n"){
            notes += line;
            QNoteButton* newNote = new QNoteButton(line.trimmed(), container);
            connect(newNote, SIGNAL (clicked()), this, SLOT (updateInputNoteText()));
            connect(newNote, SIGNAL (rightClicked()), this, SLOT (noteOptions()));
            //newNote->setStyleSheet("padding: 0px; margin: 0px;");
            notesBtns.push_back(newNote);
            btnsLayout->addWidget(newNote, Qt::AlignTop);
        }
    }
    notesLabel->setText(notes.join(" "));

    currentFile.close();
}

void MainWindow:: saveFile(){
    qDebug() << "saveFile";

    if (!currentFile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("No file to save"),
            currentFile.errorString());
        return;
    }
    QTextStream out(&currentFile);
    notes = QStringList("");
    if (btnsLayout != NULL){
        qDebug() << QString::number(notesBtns.size());
        for (int i = 0; i < notesBtns.size(); i++){
            if(notesBtns[i]){
                notes += notesBtns[i]->text();
                notes += "\n";
            }
        }
    }
    out << notes.join("");
    currentFile.close();
}

void MainWindow::saveToFile(){
    QFileDialog fileDialog(this, Qt::Dialog);
    currentFileName = QFileDialog::getSaveFileName(this,
        tr("Save Notes"), "/Andres/Text-Files", tr("Text Files (*.txt)"));
    if(currentFileName.isEmpty())
        return;
    else {
        currentFile.setFileName(currentFileName);
        fileLabelMsg = QString("Current Selected File: %1").arg(currentFileName);
        fileLabel->setText(fileLabelMsg);
        if (!currentFile.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                currentFile.errorString());
            return;
        }
        QTextStream out(&currentFile);
        out << notes.join("");
    }
}

void MainWindow::newFile(){
    notes = QStringList("");
    if (btnsLayout != NULL){
        QLayoutItem* item;
        while ((item = btnsLayout->takeAt(0)) != NULL){
            delete item->widget();
            delete item;
        }
    }
    notesBtns.clear();
    fileLabelMsg = QString("Current Selected File: [New File]");
    fileLabel->setText(fileLabelMsg);
}

void MainWindow::createActions(){
    newFileAct = new QAction(tr("&New"));
    newFileAct->setStatusTip(tr("Create a new file"));
    connect(newFileAct, &QAction::triggered, this, &MainWindow::newFile);

    openFileAct = new QAction(tr("&Open"));
    openFileAct->setStatusTip(tr("Open a file"));
    connect(openFileAct, &QAction::triggered, this, &MainWindow::openFile);

    saveFileAct = new QAction(tr("&Save"));
    saveFileAct->setStatusTip(tr("Save a file"));
    connect(saveFileAct, &QAction::triggered, this, &MainWindow::saveFile);

    saveAsFileAct = new QAction(tr("&Save As"));
    saveAsFileAct->setStatusTip(tr("Save to a file"));
    connect(saveAsFileAct, &QAction::triggered, this, &MainWindow::saveToFile);
}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newFileAct);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(saveFileAct);
    fileMenu->addAction(saveAsFileAct);
}

