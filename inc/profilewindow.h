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
#include <QCheckBox>
#include <QCloseEvent>
#include <QFile>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
	class ProfileWindow;
}

class ProfileWindow: public QMainWindow {
	Q_OBJECT

private:
	std::unique_ptr<Ui::ProfileWindow> ui;
	QMessageBox*                       aboutBox;
	QJsonObject                        profiles;
	QString                            currentProfileName;
	bool                               unsavedChanges;

	void closeEvent(QCloseEvent* event) override;
	void loadProfiles();
	void addRow(const bool active, const QString url, const QString element);
	//	void removeRow(const unsigned i);

public:
	explicit ProfileWindow(QWidget* parent = nullptr);
	virtual ~ProfileWindow();

public slots:
	void madeChange();
	void saveProfiles();
	void addRowEmpty();
};

#endif // PROFILEWINDOW_H
