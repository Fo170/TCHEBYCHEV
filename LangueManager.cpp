#include "LangueManager.hpp"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QLocale>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

const QHash<QString, QString> LangueManager::displayNames_ = {
    {"francais", "Fran\u00e7ais"},
    {"anglais",   "English"}
};

LangueManager::LangueManager(const QString& langDir, QObject* parent)
    : QObject(parent)
    , langDir_(langDir)
    , network_(new QNetworkAccessManager(this))
{
}

bool LangueManager::load(const QString& languageCode) {
    QFile f(langDir_ + "/" + languageCode + ".txt");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QHash<QString, QString> newTranslations;
    QTextStream in(&f);
    in.setEncoding(QStringConverter::Utf8);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#'))
            continue;
        int sep = line.indexOf('=');
        if (sep <= 0) continue;
        newTranslations[line.left(sep).trimmed()] = line.mid(sep + 1).trimmed();
    }
    f.close();

    translations_ = newTranslations;
    currentLang_ = languageCode;
    emit languageChanged();
    return true;
}

QString LangueManager::get(const QString& key) const {
    return translations_.value(key, QString());
}

QString LangueManager::currentLanguage() const {
    return currentLang_;
}

QStringList LangueManager::availableLanguages() const {
    return {"francais", "anglais"};
}

QStringList LangueManager::languageDisplayNames() const {
    return {"Fran\u00e7ais", "English"};
}

QString LangueManager::detectSystemLanguage() {
    QString lang = QLocale::system().name().toLower();
    if (lang.startsWith("fr")) return "francais";
    return "anglais";
}

void LangueManager::downloadLanguage(const QString& code, const QString& remoteUrl) {
    QNetworkReply* reply = network_->get(QNetworkRequest(QUrl(remoteUrl)));
    connect(reply, &QNetworkReply::finished, this, [this, reply, code]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            emit languageDownloaded(code, false);
            return;
        }
        QDir().mkpath(langDir_);
        QFile f(langDir_ + "/" + code + ".txt");
        if (!f.open(QIODevice::WriteOnly)) {
            emit languageDownloaded(code, false);
            return;
        }
        f.write(reply->readAll());
        f.close();
        emit languageDownloaded(code, true);
    });
}