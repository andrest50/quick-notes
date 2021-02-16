#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);

    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    fileLabelMsg = QString("Current Selected File: %1").arg(currentFileName);
    fileLabel = new QLabel(fileLabelMsg);
    fileLabel->setAlignment(Qt::AlignHCenter);

    QString notesLabelMsg = QString("Notes:");
    notesLabel = new QLabel(notesLabelMsg);
    notesLabel->setAlignment(Qt::AlignHCenter);

    notesBox = new QScrollArea(widget);
    notesBox->setWidget(notesLabel);
    notesBox->setWidgetResizable(true);

    inputNote = new QLineEdit(widget);
    connect(inputNote, SIGNAL (returnPressed()), this, SLOT (updateFileContent()));

    QVBoxLayout *layout = new QVBoxLayout;
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

void MainWindow::updateFileContent(){
    if(currentFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        QTextStream stream(&currentFile);
        stream << inputNote->text() << Qt::endl;
        notes += inputNote->text();
        notes += "\n";
        notesLabel->setText(notes.join(" "));
        inputNote->clear();
    }
    currentFile.close();
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
    while(!currentFile.atEnd()){
        notes += currentFile.readLine();
    }
    notesLabel->setText(notes.join(" "));

    currentFile.close();
}

void MainWindow::createActions(){
    openFileAct = new QAction(tr("&Open"));
    openFileAct->setStatusTip(tr("Open a file"));
    connect(openFileAct, &QAction::triggered, this, &MainWindow::openFile);
}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAct);
}

