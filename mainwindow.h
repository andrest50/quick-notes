#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFile>
#include <QLineEdit>
#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>
#include <vector>
#include "qnotebutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createMenus();
    void createActions();
    void clearNotes();

private:
    Ui::MainWindow* ui;
    QVBoxLayout* layout;
    QWidget* widget;

    QMenu* fileMenu;
    QAction* newFileAct;
    QAction* openFileAct;
    QAction* saveFileAct;
    QAction* saveAsFileAct;
    QKeySequence* newFileShortcut;
    QKeySequence* openFileShortcut;
    QKeySequence* saveFileShortcut;

    QLabel* fileLabel;
    QString fileLabelMsg;

    QFile currentFile;
    QString currentFileName;

    QScrollArea* notesBox;
    QLabel* notesLabel;
    QStringList notes;

    QLineEdit* inputNote;
    QString inputText;

    QWidget* container;
    QVBoxLayout* btnsLayout;
    QList<QNoteButton*> notesBtns;

    QMenu* noteMenu;
    QAction* deleteNoteAct;
    QAction* highlightNoteAct;
    QAction* editNoteAct;
    QNoteButton* noteClicked;

    QNoteButton* noteToEdit;
    bool editMode;

    QHBoxLayout* infoLayout;
    QLabel* editModeLabel;
    QLabel* savingLabel;
    QLabel* numNotesLabel;

private slots:
    void updateFileContent();
    void updateInputNoteText();
    void noteOptions();
    void newFile();
    void openFile();
    void saveFile();
    void saveToFile();
    void deleteNote();
    void highlightNote();
    void editNote();
    void displaySaving();

};
#endif // MAINWINDOW_H
