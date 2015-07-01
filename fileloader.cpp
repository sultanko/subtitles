#include "fileloader.h"
#include <QFileDialog>
#include <QtTest/QTest>
#include <QRegExp>

FileLoader::FileLoader(QObject *parent) :
    QObject(parent), file(nullptr)
{
}

FileLoader::~FileLoader()
{
    if (file != nullptr)
    {
        delete file;
    }
}

void FileLoader::loadFromFile(const QString& filename)
{
    // file hasn't chosen
    if (filename.isEmpty())
    {
        throw FileLoaderException();
    }
    // clean-up
    words.clear();
    fileText.clear();
    if (file != nullptr)
    {
        delete file;
    }
    // Open srt file to read
    file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly))
    {
        return;
    }
    QTextStream in(file);
    fileText = QString(in.readAll());
    // regex match any word from latin letters
    QRegExp regex("[a-zA-Z]+");
    int pos = regex.indexIn(fileText);
    while (pos != -1)
    {
        words.insert(regex.capturedTexts().first().toLower());
        pos += regex.matchedLength();
        pos = regex.indexIn(fileText, pos);
    }
    // leave for debug
//    while (words.size() > 20)
//    {
//        words.erase(words.begin());
//    }
}

void FileLoader::saveToFile(const QString &filename, const QMap<QString, QString> &translations)
{
    QString rfileText = fileText;
    // regex match any word from latin letters
    QRegExp regex("[a-zA-Z]+");
    int pos = regex.indexIn(rfileText);
    while (pos != -1)
    {
        // cur - world
        QString cur = regex.capturedTexts().first();
        // check if need to replace this world
        if (translations.contains(cur.toLower())
                // don't replace words from html coloring
                && cur != "font"
                && cur != "color"
                && cur != "ff")
        {
            QString replCur(cur +
                              " (<font color=\"#80ff80\">" + translations.find(cur.toLower()).value() + "</font>)");
            rfileText.replace(pos, regex.matchedLength(), replCur);
            pos += replCur.length() - regex.matchedLength();
        }
        pos += regex.matchedLength();
        pos = regex.indexIn(rfileText, pos);
    }
    // write replaced file
    QFile savedFile(filename);
    if (!savedFile.open(QIODevice::WriteOnly)) {
        return;
    }
    QTextStream out(&savedFile);
    out << rfileText;
}
