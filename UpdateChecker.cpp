#include "UpdateChecker.hpp"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

UpdateChecker::UpdateChecker(const QString& currentVersion,
                             const QString& checkUrl,
                             QObject* parent)
    : QObject(parent)
    , manager_(new QNetworkAccessManager(this))
    , currentVersion_(currentVersion)
    , checkUrl_(checkUrl)
{
}

void UpdateChecker::checkForUpdates() {
    if (checking_) return;
    checking_ = true;

    QNetworkRequest request{QUrl(checkUrl_)};
    request.setHeader(QNetworkRequest::UserAgentHeader, "TCHEBYCHEV/2.0");
    request.setTransferTimeout(10000);

    QNetworkReply* reply = manager_->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onReplyFinished(reply);
    });
}

bool UpdateChecker::isChecking() const {
    return checking_;
}

void UpdateChecker::onReplyFinished(QNetworkReply* reply) {
    checking_ = false;

    if (reply->error() != QNetworkReply::NoError) {
        emit checkError(reply->errorString());
        reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();

    if (!doc.isObject()) {
        emit checkError("Réponse JSON invalide");
        return;
    }

    QJsonObject obj = doc.object();
    QString latestVer = obj["version"].toString();
    QString downloadUrl = obj["url"].toString();
    QString releaseNotes = obj["notes"].toString();

    QVersionNumber current = QVersionNumber::fromString(currentVersion_);
    QVersionNumber latest = QVersionNumber::fromString(latestVer);

    if (latest > current)
        emit updateAvailable(latestVer, downloadUrl, releaseNotes);
    else
        emit upToDate();
}