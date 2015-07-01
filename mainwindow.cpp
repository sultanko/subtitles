#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    errorMessage = new QErrorMessage(this);
//    connect(&this->translator, SIGNAL(translatedList()), this, SLOT(on_transleted()));
    connect(&this->translator, &Translator::translatedList, this, &MainWindow::on_transleted);
}

MainWindow::~MainWindow()
{
    delete errorMessage;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    translator.translate("father mother brother");
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open subtitles"),
                "",
                tr("Subtitles (*.srt);;All files (*)"));
    dialog.reset(new QProgressDialog("Translating words", "Cancel", 0, 0, this));
    dialog->show();
    try
    {
        fileLoader.loadFromFile(filename);
        translator.translateList(fileLoader.getWords().toList());
    }
    // if error occurs with file
    catch (const FileLoaderException& e)
    {
        dialog.reset();
        errorMessage->showMessage("Can't load file");
        return;
    }
    // if error occurs while translating
    catch (const TranslatorException& e)
    {
        dialog.reset();
        errorMessage->showMessage("Network error");
        return;
    }
}

void MainWindow::on_transleted()
{
    const QMap<QString, QPair<QString, qint32>>& translations = translator.getMapWords();
    QTableWidget* pTable = ui->tableWidget;
    pTable->setRowCount(0);
    // cancel showing dialog
    dialog.reset();
    for (auto translation = translations.begin(); translation != translations.end(); ++translation)
    {
        QTableWidgetItem* checkBox = new QTableWidgetItem();
        if (translation.value().second == (qint32)Translator::WordState::ToStudy)
        {
            checkBox->setCheckState(Qt::Checked);
        }
        else
        {
            checkBox->setCheckState(Qt::Unchecked);
        }
        QTableWidgetItem* wordOriginal = new QTableWidgetItem();
        wordOriginal->setText(translation.key());
        QTableWidgetItem* wordTr = new QTableWidgetItem();
        wordTr->setText(translation.value().first);
        int rowNumber = pTable->rowCount();
        pTable->insertRow(rowNumber);
        pTable->setItem(rowNumber, 0, wordOriginal);
        pTable->setItem(rowNumber, 1, wordTr);
        pTable->setItem(rowNumber, 2, checkBox);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
                this,
                tr("Open subtitles"),
                "",
                tr("Subtitles (*.srt);;All files (*)"));
    QMap<QString, QString> translations;
    QMap<QString, QString> addTranslations;
    QTableWidget* pTable = ui->tableWidget;
    for (int i =  0; i < pTable->rowCount(); ++i)
    {
        if (pTable->item(i, 2)->checkState() == Qt::Checked)
        {
            translations.insert(pTable->item(i, 0)->text(),
                                pTable->item(i, 1)->text());
        }
        else
        {
            addTranslations.insert(pTable->item(i, 0)->text(),
                                pTable->item(i, 1)->text());
        }
    }
    try
    {
        fileLoader.saveToFile(filename, translations);
        translator.updateDictionary(addTranslations);
    }
    catch (const FileLoaderException& e)
    {
        errorMessage->showMessage("Can't save file");
        return;
    }
    catch (const TranslatorException& e)
    {
        errorMessage->showMessage("Can't update dictionary");
        return;
    }
}

void MainWindow::on_actionOpen_file_triggered()
{
   on_pushButton_clicked();
}

void MainWindow::on_actionSave_file_triggered()
{
   on_pushButton_2_clicked();
}
