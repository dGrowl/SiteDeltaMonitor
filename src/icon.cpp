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
		menu(nullptr),
		win(nullptr),
		timer(this) {
		setIcon(QIcon(":/res/icon.png"));
		createMenu();
		connect(this, &QSystemTrayIcon::activated, this, &Icon::handleClick);
		timer.callOnTimeout(this, &Icon::runTests);
		int interval = minsToMillis(30);
		QFile profilesFile("profiles.json");
		if (profilesFile.exists()) {
			profilesFile.open(QIODevice::ReadOnly | QIODevice::Text);
			QJsonObject profiles = QJsonDocument::fromJson(profilesFile.readAll()).object();
			for (auto it = profiles.begin(); it != profiles.end(); ++it) {
				QJsonObject profile = it.value().toObject();
				if (profile.value("current").toBool() == true) {
					interval = minsToMillis(profile.value("interval").toInt());
					break;
				}
			}
			profilesFile.close();
		}
		setInterval(interval);
		setActive(true);
	}

	void Icon::createMenu() {
		try {
			menu = std::make_unique<QMenu>();
			setContextMenu(menu.get());
			QAction* activeAction  = menu->addAction("&Active");
			QAction* optionsAction = menu->addAction("&Options");
			menu->addSeparator();
			QAction* exitAction = menu->addAction("&Exit");
			activeAction->setCheckable(true);
			activeAction->setChecked(true);
			connect(activeAction,  &QAction::toggled,   this,                     &Icon::setActive);
			connect(optionsAction, &QAction::triggered, this,                     &Icon::openConfig);
			connect(exitAction,    &QAction::triggered, QApplication::instance(), &QApplication::quit);
		}
		catch (std::exception& e) {
			qDebug() << e.what() << "Error: Failed to construct menu for SDM::Icon.";
			deleteLater();
		}
	}

	void Icon::handleClick(const QSystemTrayIcon::ActivationReason reason) {
		if (reason == QSystemTrayIcon::DoubleClick) {
			openConfig();
		}
	}

	void Icon::showNotification(const QString& urlString) {
		showMessage("SDM: Difference Detected", "SDM has detected a change at\n" + urlString);
	}

	void Icon::openConfig() {
		if (win.isNull()) {
			win = new ProfileWindow();
			win->setWindowIcon(QIcon(":/res/icon.png"));
			win->setAttribute(Qt::WA_DeleteOnClose, true);
			win->setAttribute(Qt::WA_QuitOnClose,   false);
		}
		win->show();
		win->activateWindow();
		win->raise();
	}

	void Icon::runTests() {
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
							try {
								Check* test = new Check(target.value("url").toString(), target.value("element").toString(), this);
								test->run();
								connect(test, &Check::difference, this, &Icon::showNotification);
							}
							catch (std::exception& e) {
								qDebug() << "Fatal Error: Failed to create an SDM::Test.";
								std::abort();
							}
						}
					}
					break;
				}
			}
			profilesFile.close();
		}
	}

	void Icon::setActive(const bool beActive) {
		if (beActive) {
			timer.start();
		}
		else {
			timer.stop();
		}
	}

	void Icon::setInterval(const int ms) {
		if (timer.isActive()) {
			timer.stop();
			timer.setInterval(ms);
			timer.start();
		}
		else {
			timer.setInterval(ms);
		}
	}
}
