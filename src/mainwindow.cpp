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

#include "inc/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
	QMainWindow(parent),
	ui(std::make_unique<Ui::MainWindow>()),
	aboutBox(new QMessageBox(this)),
	profiles(),
	currentProfileName() {
	ui->setupUi(this);
	loadProfiles();

	aboutBox->setWindowTitle("SDM: About");
	aboutBox->setText("This project was created by github.com/dGrowl.");
	aboutBox->setInformativeText("Created using Qt.\nQt is Copyright © 2019 The Qt Company.");

	connect(ui->action_About,  &QAction::triggered,   aboutBox, &QMessageBox::exec);
	connect(ui->button_Save,   &QPushButton::clicked, this,     &MainWindow::saveProfiles);
	connect(ui->button_AddRow, &QPushButton::clicked, this,     [=]() {
		addRow();
	});
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent* event) {
//	TODO: Handle unsaved profile changes
	emit exit();
	event->accept();
}

void MainWindow::addRow(const bool active, const QString url, const QString element) {
	int gridRowCount = ui->layout_Config_G->rowCount();
	QCheckBox* rowActiveToggle = new QCheckBox(this);
	rowActiveToggle->setChecked(active);
	ui->layout_Config_G->addWidget(rowActiveToggle, gridRowCount, 0, 1, 1, Qt::AlignRight);
	ui->layout_Config_G->addWidget(new QLineEdit(url, this), gridRowCount, 1);
	ui->layout_Config_G->addWidget(new QLineEdit(element, this), gridRowCount, 2);
}

//void MainWindow::removeRow(const unsigned i) {
	// TODO: Implement
	// Remove ith row of internal 2D QLayoutItem pointer array
	// Call method that deletes and replaces existing layout
//}

void MainWindow::loadProfiles() {
	QFile profilesFile("profiles.json");
	if (profilesFile.exists()) {
		// TODO: Validate profiles json
		profilesFile.open(QIODevice::ReadOnly | QIODevice::Text);
		profiles = QJsonDocument::fromJson(profilesFile.readAll()).object();
	}
	else {
		profilesFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
		QJsonObject defaultProfile;
		defaultProfile.insert("current", true);
		defaultProfile.insert("interval", 30);
		QJsonObject defaultTarget;
		defaultTarget.insert("active", true);
		defaultTarget.insert("url", "https://www.google.com");
		defaultTarget.insert("element", "");
		QJsonArray profileTargets = { defaultTarget };
		defaultProfile.insert("targets", profileTargets);
		profiles.insert("Profile &1", defaultProfile);
		profilesFile.write(QJsonDocument(profiles).toJson());
	}
	profilesFile.close();
	for (auto it = profiles.constBegin(); it != profiles.constEnd(); ++it) {
		QString profileName     = it.key();
		QJsonObject profileData = it.value().toObject();
		QAction* profileAction  = new QAction(profileName, ui->menu_Profiles);
		ui->menu_Profiles->insertAction(ui->action_New, profileAction);
		if (profileData.value("current").toBool()) {
			profileAction->setCheckable(true);
			profileAction->setChecked(true);
			currentProfileName = profileName;
			QJsonArray targets = profileData.value("targets").toArray();
			for (int i = 0; i < targets.size(); ++i) {
				QJsonObject target = targets[i].toObject();
				addRow(
					target.value("active").toBool(), 
					target.value("url").toString(), 
					target.value("element").toString()
				);
			}
		}
	}
	ui->menu_Profiles->insertSeparator(ui->action_New);
}

void MainWindow::saveProfiles() {
	int gridRowCount = ui->layout_Config_G->rowCount();
	QJsonObject profile;
	profile.insert("current", true);
	int interval = (ui->spinBox_Hours->value() * 60) + ui->spinBox_Minutes->value();
	if (interval < 10) {
		interval = 10; // Reasonable minimum of 10 minutes between updates for now.
	}
	profile.insert("interval", interval);
	QJsonArray targets;
	for (int i = 1; i < gridRowCount; ++i) {
		bool    rowActive  = qobject_cast<QCheckBox*>(ui->layout_Config_G->itemAtPosition(i, 0)->widget())->isChecked();
		QString rowURL     = qobject_cast<QLineEdit*>(ui->layout_Config_G->itemAtPosition(i, 1)->widget())->text();
		QString rowElement = qobject_cast<QLineEdit*>(ui->layout_Config_G->itemAtPosition(i, 2)->widget())->text();
		QJsonObject target;
		target.insert("active",  rowActive);
		target.insert("url",     rowURL);
		target.insert("element", rowElement);
		targets.append(target);
	}
	profile.insert("targets", targets);
	profiles.remove(currentProfileName);
	profiles.insert(currentProfileName, profile);
	QJsonDocument profilesDoc(profiles);
	QFile profilesFile("profiles.json");
	profilesFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	profilesFile.write(profilesDoc.toJson());
	profilesFile.close();
}
