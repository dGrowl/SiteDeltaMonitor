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

#ifndef CHECK_H
#define CHECK_H

#include <QDir>
#include <QNetworkReply>
#include <QPointer>
#include <gumbo.h>
#include "inc/reportwindow.h"

namespace SDM {
	class Check: public QObject {
		Q_OBJECT

	private:
		QNetworkAccessManager net;
		QNetworkReply*        reply;
		QString               urlString;
		QString               elementString;
		QString               prevData;
		QString               liveData;
		void                  collectElements(const std::string& html, QString& data, GumboNode* node);
		void                  collectElements(const QString& html, QString& data);

	public:
		Check(const QString& urlP, const QString& elementP, QObject* parent = nullptr);
		virtual ~Check();
		void run();
		void report(const QString& previous, const QString& current);
		void compare();

	public slots:
		void process();

	signals:
		void difference(const QString& url);
	};
}

#endif // CHECK_H
