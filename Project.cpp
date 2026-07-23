#include "Project.hpp"
#include <QFile>
#include <QJsonDocument>

Project::Project() {
    data_["version"] = "1.0";
    data_["name"] = "TCHEBYCHEV";
    data_["components"] = QJsonArray();
}

bool Project::load(const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();

    if (!doc.isObject()) return false;

    data_ = doc.object();
    file_path_ = path;
    modified_ = false;
    return true;
}

bool Project::save(const QString& path) const {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
        return false;

    QJsonDocument doc(data_);
    f.write(doc.toJson(QJsonDocument::Indented));
    f.close();

    const_cast<Project*>(this)->file_path_ = path;
    const_cast<Project*>(this)->modified_ = false;
    return true;
}

void Project::set_name(const QString& name) {
    data_["name"] = name;
    modified_ = true;
}

QString Project::name() const {
    return data_["name"].toString();
}

void Project::add_component(const QJsonObject& component) {
    QJsonArray arr = data_["components"].toArray();
    arr.append(component);
    data_["components"] = arr;
    modified_ = true;
}

QJsonArray Project::components() const {
    return data_["components"].toArray();
}

void Project::set_data(const QJsonObject& data) {
    data_ = data;
    modified_ = true;
}

QJsonObject Project::data() const {
    return data_;
}

bool Project::is_modified() const {
    return modified_;
}

void Project::set_modified(bool modified) {
    modified_ = modified;
}

QString Project::file_path() const {
    return file_path_;
}