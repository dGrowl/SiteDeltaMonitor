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

#ifndef ICON_H
#define ICON_H

#include <memory>
#include <QApplication>
#include <QIcon>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QSystemTrayIcon>
#include <QTimer>
#include "inc/mainwindow.h"
#include "inc/monitor.h"
#include "inc/reportwindow.h"

namespace SDM {
	class Icon: public QSystemTrayIcon {
		Q_OBJECT

	private:
		std::unique_ptr<MainWindow> win;
		std::unique_ptr<QMenu>      menu;
		Monitor* monitor;
		QTimer   timer;
		void createMenu();

	public:
		Icon(QObject* parent = nullptr);
		virtual ~Icon();

	public slots:
		void handleClick(const QSystemTrayIcon::ActivationReason reason);
		void generateReport(const QString url, const QString previous, const QString current);
		void openConfig();
		void exitConfig();
		void runMonitor();
		void setActive(bool beActive);
		void setInterval(const int ms);
	};
}

#endif // ICON_H
