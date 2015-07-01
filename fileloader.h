#ifndef FILELOADER_H
#define FILELOADER_H

#include <QObject>
#include <QString>
#include <QSet>
#include <QMap>
#include <QFile>
#include <QException>

class FileLoaderException : public QException
{
public:
    FileLoaderException()
        : QException()
    {}

private:
};

class FileLoader : public QObject
{
    Q_OBJECT
public:
    explicit FileLoader(QObject *parent = 0);
    virtual ~FileLoader();
    // load srt file and parse words from it
    // blocking
    void loadFromFile(const QString& filename);

    // save to srt file and replacing words in it
    // by given rules
    // blocking
    void saveToFile(const QString& filename, const QMap<QString, QString>& translations);

    // get words from file
    const QSet<QString>& getWords() { return words; }


signals:

public slots:

private:
    QFile* file;
    QString fileText;
    QSet<QString> words;
};

#endif // FILELOADER_H
