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

#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <memory>
#include <QActionGroup>
#include <QCheckBox>
#include <QCloseEvent>
#include <QDebug>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
	class ProfileWindow;
}

namespace SDM {
	class ProfileWindow: public QMainWindow {
		Q_OBJECT

	private:
		std::unique_ptr<Ui::ProfileWindow> ui;
		QActionGroup* profileActions;
		QAction* separator_Profile; // KLUGE: profilewindow.ui hates separators.
		QMessageBox* aboutBox;
		QJsonObject profiles;
		QString currentProfileName;
		bool unsavedChanges;
		void closeEvent(QCloseEvent* event) override;
		void shrinkToFit();
		void setActiveProfile(QAction* action);
		void loadProfiles();
		void addTarget(const bool active, const QString& url, const QString& element);
		void removeTarget(QAction* target);
		void clearTargets();

	public:
		explicit ProfileWindow(QWidget* parent = nullptr);
		ProfileWindow(const ProfileWindow& other)            = delete;
		ProfileWindow& operator=(const ProfileWindow& other) = delete;
		virtual ~ProfileWindow();

	public slots:
		void madeChange();
		void addProfile();
		void saveProfiles();
		void addTargetBlank();
		void removalMenu();
	};
}

#endif // PROFILEWINDOW_H
