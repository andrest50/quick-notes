#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFile>
#include <QLineEdit>
#include <QScrollArea>

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

private:
    Ui::MainWindow *ui;
    QMenu* fileMenu;
    QAction* openFileAct;
    QAction* saveFileAct;

    QLabel* fileLabel;
    QString fileLabelMsg;

    QFile currentFile;
    QString currentFileName;

    QScrollArea* notesBox;
    QLabel* notesLabel;
    QStringList notes;

    QLineEdit* inputNote;
    QString inputText;

private slots:
    void updateFileContent();
    void openFile();
    void saveToFile();

};
#endif // MAINWINDOW_H
