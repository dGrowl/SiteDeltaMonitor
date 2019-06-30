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

#ifndef MONITOR_H
#define MONITOR_H

#include <fstream>
#include <memory>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>

namespace SDM {
	class Monitor: public QObject {
		Q_OBJECT

	private:
		QNetworkAccessManager          net;
		QJsonObject                    history;
		QHash<QString, QNetworkReply*> tests;

		void loadHistory();
		void saveHistory();
		void loadProfile();

	public:
		Monitor(QObject* parent = nullptr);
		virtual ~Monitor();
		void fetch(std::shared_ptr<QString> urlStringPtr);
		void compare(std::shared_ptr<QString> urlStringPtr);

	signals:
		void difference(std::shared_ptr<QString> urlStringPtr, const QString& previous, const QString& current);
	};
}

#endif // MONITOR_H
