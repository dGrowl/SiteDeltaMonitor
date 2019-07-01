/*
 * Copyright 2019, github.com/dGrowl
 *
 * This file is part of SDM.
 *
 * SDM is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * SDM is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SDM. If not, see <https://www.gnu.org/licenses/>.
 */

#include "inc/monitor.h"

namespace SDM {
	Monitor::Monitor(QObject* parent):
		QObject(parent),
		tests() {
		loadProfile();
	}

	Monitor::~Monitor() {}

	void Monitor::loadProfile() {
		QFile profilesFile("profiles.json");
		if (profilesFile.exists()) {
			profilesFile.open(QIODevice::ReadOnly | QIODevice::Text);
			QJsonObject profiles = QJsonDocument::fromJson(profilesFile.readAll()).object();
			for (auto it = profiles.begin(); it != profiles.end(); ++it) {
				QJsonObject profile = it.value().toObject();
				if (profile.value("current").toBool() == true) {
					QJsonArray targets = profile.value("targets").toArray();
					for (int i = 0; i < targets.size(); ++i) {
						QJsonObject target = targets[i].toObject();
						if (target.value("active") == true) {
							std::shared_ptr<QString> urlString = std::make_shared<QString>(target.value("url").toString());
							fetch(urlString);
						}
					}
					break;
				}
			}
			profilesFile.close();
		}
	}

	void Monitor::fetch(std::shared_ptr<QString> urlStringPtr) {
		QString& urlString = *urlStringPtr.get();
		QUrl url(urlString);
		qDebug() << "Fetching URL:" << urlString;
		if (!url.isValid() || url.isLocalFile() || url.isRelative()) {
			qDebug() << "Error: Invalid URL" << urlString;
			return;
		}
		QNetworkReply* reply = net.get(QNetworkRequest(url));
		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Error: Invalid HTTP reply" << reply->errorString();
			return;
		}
		tests.insert(urlString, reply);
		connect(reply, &QNetworkReply::finished, this, [=]() {
			compare(urlStringPtr);
		});
	}

	void Monitor::compare(std::shared_ptr<QString> urlStringPtr) {
		QString& urlString = *urlStringPtr.get();
		qDebug() << "Attempting to compare data for" << urlString << "with previous data.";
		QNetworkReply* reply = tests.value(urlString);
		int replyStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (replyStatus != 200) {
			qDebug() << "Error: Invalid HTTP reply code" << QVariant(replyStatus).toInt();
		}
		else {
			QString liveData = reply->readAll();
			QCryptographicHash urlHash(QCryptographicHash::Md5);
			urlHash.addData(urlString.toUtf8());
			QString urlHash64 = urlHash.result().toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
			QDir().mkdir("history");
			QString historyFileString = QDir::toNativeSeparators("history/" + urlHash64);
			QFile historyFile(historyFileString);
			if (historyFile.exists()) {
				if (historyFile.open(QIODevice::ReadOnly)) {
					QString prevData = historyFile.readAll();
					historyFile.close();
					if (prevData != liveData) {
						emit difference(urlStringPtr, prevData, liveData);
					}
				}
				else {
					qDebug() << "Error: History file exists but could not be read from." << historyFile.errorString();
				}
			}
			if (historyFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
				historyFile.write(liveData.toUtf8());
				historyFile.close();
			}
			else {
				qDebug() << "Error: Failed to write to history file." << historyFile.errorString();
			}
		}
		reply->close();
		tests.remove(urlString);
		if (tests.isEmpty()) {
			deleteLater();
		}
	}
}
