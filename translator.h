#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QString>
#include <QNetworkAccessManager>
#include <QMap>
#include <QMutex>
#include <QAtomicInt>
#include <QPair>
#include <QException>
#include "myatomicinteger.h"

class TranslatorException : public QException
{
public:
    TranslatorException()
        : QException()
    {}

private:
};

class Translator : public QObject
{
    Q_OBJECT

public:
    Translator();
    virtual ~Translator() {}

    // translate list of words
    // non-blocking
    // signal translatingList emitted when finished
    void translateList(const QStringList& list);

    // add to dictionary new words with their translations
    void updateDictionary(const QMap<QString, QString>& newWords);

    const QMap<QString, QPair<QString, qint32>>& getMapWords() { return listTr; }
    const QMap<QString, QPair<QString, qint32>>& getKnownWords() { return knownWords; }

    enum class WordState
    {
        Known,
        ToStudy
    };

signals:
    // signal called when parsed word and it translation
    void translated();
    // signal called when list of worlds translated
    void translatedList();

private slots:
    // Slot called when answer come from server
    void on_finished_request(QNetworkReply* reply);
    // slot calling when word from list translated
    void translatingList();

private:
    // translate given text from en to ru
    // slot on_finished_request called when finished translating
    void translate(
            const QString& text,
            const QString& from = "en",
            const QString& to = "ru"
                      );


    QNetworkAccessManager http; // http-server for translation
    QStringList list;
    size_t listSize;
    MyAtomicInteger translatedCount;
    QMap<QString, QPair<QString, qint32>> listTr; // current words with their translations
    QMap<QString, QPair<QString, qint32>> knownWords; // all words with their translations
    QMutex mutex;

    static const QString DICTIONARY_NAME; // filename of dictionary
    static const QString API_KEY; // api key for this application for yandex.api
    static const QString REQUEST_URL; // url from yandex.api
};

#endif // TRANSLATOR_H
