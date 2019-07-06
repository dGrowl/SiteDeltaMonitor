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
		net(this),
		reply(nullptr),
		urlString(urlStringP),
		elementString(elementStringP),
		prevData(),
		liveData() {}

	Check::~Check() {
		reply->close();
	}

	void Check::collectElements(const std::string& html, QString& data, GumboNode* node) {
		if (node->type == GUMBO_NODE_ELEMENT) {
			GumboElement& elem = node->v.element;
			bool desired = false;
			if (elementString.at(0) == '#') {
				GumboAttribute* id = gumbo_get_attribute(&elem.attributes, "id");
				desired = (id != nullptr && elementString.right(elementString.size() - 1) == id->value);
			}
			else if (elementString.at(0) == '.') {
				GumboAttribute* classes = gumbo_get_attribute(&elem.attributes, "class");
				if (classes != nullptr) {
					QString classesString(classes->value);
					QStringList classesList = classesString.split(' ', QString::SkipEmptyParts);
					desired = classesList.contains(elementString.right(elementString.size() - 1));
				}
			}
			else {
				const char* type = gumbo_normalized_tagname(elem.tag);
				desired = (elementString == type);
			}
			if (desired) {
				if (elem.original_end_tag.length == 0) {
					data += html.substr(
						elem.start_pos.offset,
						elem.original_tag.length
					).c_str();
				}
				else {
					data += html.substr(
						elem.start_pos.offset,
						elem.end_pos.offset - elem.start_pos.offset + elem.original_end_tag.length
					).c_str();
				}
				data += '\n';
			}
			else {
				for (unsigned i = 0; i < elem.children.length; ++i) {
					collectElements(html, data, static_cast<GumboNode*>(elem.children.data[i]));
				}
			}
		}
	}

	void Check::collectElements(const QString& html, QString& data) {
		if (elementString.isEmpty()) {
			data = html;
		}
		else {
			std::string html2 = html.toUtf8().data();
			GumboOutput* output = gumbo_parse(html2.c_str());
			if (output == nullptr) {
				qDebug() << "Error: Major failure to parse HTML.";
			}
			else {
				if (output->errors.length != 0) {
					qDebug() << "Errors occured during parsing.";
				}
				collectElements(html2, data, output->root);
				gumbo_destroy_output(&kGumboDefaultOptions, output);
			}
		}
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
		connect(reply, &QNetworkReply::finished, this, &Check::process);
	}

	void Check::report(const QString& previous, const QString& current) {
		QPointer<ReportWindow> report = new(std::nothrow) ReportWindow(urlString, previous, current);
		if (!report.isNull()) {
			report->show();
		}
		emit difference(urlString);
	}

	void Check::compare() {
		if (prevData != liveData) {
			report(prevData, liveData);
		}
		deleteLater();
	}

	void Check::process() {
		qDebug() << "Attempting to compare data for" << urlString << "with previous data.";
		int replyStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (replyStatus != 200) {
			qDebug() << "Error: Invalid HTTP reply code" << QVariant(replyStatus).toInt();
		}
		else {
			QString liveHTML = reply->readAll();
			QCryptographicHash urlHash(QCryptographicHash::Md5);
			urlHash.addData(urlString.toUtf8());
			QString urlHash64 = urlHash.result().toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
			QDir().mkdir("history");
			QString historyFileString = QDir::toNativeSeparators("history/" + urlHash64);
			QFile historyFile(historyFileString);
			if (historyFile.exists()) {
				if (historyFile.open(QIODevice::ReadOnly)) {
					QString prevHTML = historyFile.readAll();
					historyFile.close();
					collectElements(prevHTML, prevData);
					collectElements(liveHTML, liveData);
					compare();
				}
				else {
					qDebug() << "Error: History file exists but could not be read from." << historyFile.errorString();
				}
			}
			if (historyFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
				historyFile.write(liveHTML.toUtf8());
				historyFile.close();
			}
			else {
				qDebug() << "Error: Failed to write to history file." << historyFile.errorString();
			}
		}
	}
}
