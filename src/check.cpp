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

#include "inc/check.h"

namespace SDM {
	Check::Check(const QString& urlStringP, const QString& elementStringP, QObject* parent):
		QObject(parent),
		net(),
		reply(nullptr),
		urlString(urlStringP),
		elementString(elementStringP) {}

	Check::~Check() {
		reply->close();
	}

	void Check::run() {
		qDebug() << "Fetching URL:" << urlString;
		QUrl url(urlString);
		if (!url.isValid() || url.isLocalFile() || url.isRelative()) {
			qDebug() << "Error: Invalid URL" << urlString;
			deleteLater();
			return;
		}
		reply = net.get(QNetworkRequest(url));
		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Error: Invalid HTTP reply" << reply->errorString();
			deleteLater();
			return;
		}
		connect(reply, &QNetworkReply::finished, this, &Check::compare);
	}

	void Check::report(const QString& previous, const QString& current) {
		QPointer<ReportWindow> report = new(std::nothrow) ReportWindow(urlString, previous, current);
		if (!report.isNull()) {
			report->show();
		}
		emit difference(urlString);
	}

	void Check::compare() {
		qDebug() << "Attempting to compare data for" << urlString << "with previous data.";
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
						report(prevData, liveData);
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
		deleteLater();
	}
}
