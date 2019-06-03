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

#include "inc/icon.h"

namespace SDM {
	Icon::Icon(QObject* parent):
		QSystemTrayIcon(parent),
		win(nullptr),
		menu(new QMenu(nullptr)) {
		setIcon(QIcon(":/res/icon.png"));
		setContextMenu(menu);
		connect(this, &QSystemTrayIcon::activated, this, &Icon::handleClick);
		QAction* activeAction = menu->addAction(tr("&Active"));
		activeAction->setCheckable(true);
		QAction* optionsAction = menu->addAction(tr("&Options"));
		menu->addSeparator();
		QAction* exitAction = menu->addAction(tr("&Exit"));
		connect(optionsAction, &QAction::triggered, this, &Icon::openConfig);
		connect(exitAction, &QAction::triggered, QApplication::instance(), &QApplication::quit);
	}

	Icon::~Icon() {
		delete menu;
		delete win;
	}

	void Icon::handleClick(QSystemTrayIcon::ActivationReason reason) {
		if (reason == QSystemTrayIcon::DoubleClick) {
			openConfig();
		}
	}

	void Icon::openConfig() {
		if (win == nullptr) {
			win = new MainWindow();
			win->setWindowIcon(QIcon(":/res/icon.png"));
		}
		win->show();
		// win->activateWindow(); TODO: Refactor QMainWindow
		win->raise();
	}

	void Icon::exitConfig() {
		delete win;
	}
}
