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

#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>
#include "inc/profilewindow.h"
#include "inc/check.h"

namespace SDM {
	inline int minsToMillis(const int mins) {
		return (mins * 60 * 1000);
	}

	class Icon: public QSystemTrayIcon {
		Q_OBJECT

	private:
		std::unique_ptr<QMenu>  menu;
		QPointer<ProfileWindow> win;
		QTimer                  timer;
		void createMenu();

	public:
		Icon(QObject* parent = nullptr);
		virtual ~Icon() = default;

	public slots:
		void handleClick(const QSystemTrayIcon::ActivationReason reason);
		void showNotification(const QString& urlString);
		void openConfig();
		void runTests();
		void setActive(const bool beActive);
		void setInterval(const int ms);
	};
}

#endif // ICON_H
