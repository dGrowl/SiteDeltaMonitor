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
#include <QIcon>
#include <QMenu>
#include <QSystemTrayIcon>
#include "inc/mainwindow.h"

namespace SDM {
	class Icon: public QSystemTrayIcon {
		Q_OBJECT

	private:
		MainWindow* win;
		QMenu* menu;

	public:
		Icon(QObject* parent = nullptr);
		virtual ~Icon();

	public slots:
		void handleClick(QSystemTrayIcon::ActivationReason reason);
		void openConfig();
		void exitConfig();
	};
}

#endif // ICON_H
