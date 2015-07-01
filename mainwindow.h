#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "translator.h"
#include "fileloader.h"
#include <QProgressDialog>
#include <QErrorMessage>
#include <memory>
#include <QSharedPointer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_transleted();

    void on_pushButton_2_clicked();

    void on_actionOpen_file_triggered();

    void on_actionSave_file_triggered();

private:
    Ui::MainWindow *ui;
    QErrorMessage* errorMessage;
    QSharedPointer<QProgressDialog> dialog;
    Translator translator;
    FileLoader fileLoader;
};

#endif // MAINWINDOW_H
