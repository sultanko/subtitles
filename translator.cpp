#include "translator.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <iostream>
#include <QDebug>
#include <QtTest>
#include <QtCore>

const QString Translator::DICTIONARY_NAME = "words.txt";

const QString Translator::API_KEY = "trnsl.1.1.20150517T163021Z.d917de8171174715.f1e3dcb1fdd853f7872d11e821bc42206383c3d8";
const QString Translator::REQUEST_URL =
        "https://translate.yandex.net/api/v1.5/tr.json/translate"
        "?key=%0"
        "&lang=%1-%2"
        "&text=%3";

Translator::Translator()
{
    connect(&http, &QNetworkAccessManager::finished, this, &Translator::on_finished_request);
    connect(this, &Translator::translated, this, &Translator::translatingList);

    // Load dictionary
    QFile dictionary(DICTIONARY_NAME);
    if (!dictionary.open(QIODevice::ReadOnly))
    {
        return;
    }
    QDataStream in(&dictionary);
    in.setVersion(QDataStream::Qt_5_2);
    in >> knownWords;
}

void Translator::translate(
        const QString& text,
        const QString& from,
        const QString& to)
{
    // Try to find word in dictionary
    if (knownWords.contains(text.toLower())) {
        listTr.insert(text, knownWords.find(text).value());
        emit translated();
        return;
    }
    // Send request to yandex translater
    QString url = REQUEST_URL.arg(API_KEY).arg(from).arg(to).arg(text);
    QNetworkRequest request(url);
    http.get(request);
}

void Translator::on_finished_request(QNetworkReply* reply)
{

    // Take original word from request
    QString textOriginal = reply->url().toString();
    int posSymb = textOriginal.lastIndexOf("=");
    textOriginal.remove(0, posSymb + 1);

    QString strReply(reply->readAll());
    reply->deleteLater();
    // Parsing json answer
    QJsonDocument document = QJsonDocument::fromJson(strReply.toUtf8());
    reply->deleteLater();
    QJsonObject jobject = document.object();
    QJsonArray array = jobject["text"].toArray();
    QString result;
    // can be multiply translations for given word
    for (auto it = array.begin(); it != array.end(); ++it)
    {
        if (!result.isEmpty())
        {
            result.append("|");
        }
        result += (*it).toString();
    }
    // Remebmer word with this translation
    mutex.lock();
    textOriginal = textOriginal.toLower();
    listTr.insert(textOriginal, QPair<QString, qint32>(result, (qint32)WordState::ToStudy));
    if (!knownWords.contains(textOriginal))
    {
        knownWords.insert(textOriginal, QPair<QString, qint32>(result, (qint32)WordState::ToStudy));
    }
    mutex.unlock();
    emit translated();
}

void Translator::translatingList()
{
    // increment count of translated
    ++translatedCount;
    if (translatedCount.get() == list.size())
    {
        list.clear();
        emit translatedList();
    }
}

void Translator::translateList(const QStringList &list)
{
    this->listSize = list.size();
    // creating copy of list for access from QNetworkManager
    this->list = list;
    this->translatedCount.set(0);
    this->listTr.clear();
    for (QString item : this->list)
    {
        translate(item);
    }
}


void Translator::updateDictionary(const QMap<QString, QString> &newWords)
{
    // update map of worlds
    for (auto iter = newWords.begin(); iter != newWords.end(); ++iter)
    {
        if (knownWords.contains(iter.key()))
        {
            knownWords.insert(iter.key(), QPair<QString, qint32>(iter.value(), (qint32)WordState::Known));
        }
    }
    QFile dictionary(DICTIONARY_NAME);
    if (!dictionary.open(QIODevice::WriteOnly))
    {
        return;
    }
    // serialize map
    QDataStream out(&dictionary);
    out.setVersion(QDataStream::Qt_5_2);
    out << knownWords;
}
