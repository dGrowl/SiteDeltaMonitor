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

#include "inc/profilewindow.h"
#include "ui_profilewindow.h"

ProfileWindow::ProfileWindow(QWidget *parent):
	QMainWindow(parent),
	ui(std::make_unique<Ui::ProfileWindow>()),
	aboutBox(new QMessageBox(this)),
	profiles(),
	currentProfileName(),
	unsavedChanges(false) {
	ui->setupUi(this);
	loadProfiles();
	aboutBox->setWindowTitle("SDM: About");
	aboutBox->setText("This project was created by github.com/dGrowl.");
	aboutBox->setInformativeText("Created using Qt.\nQt is Copyright © 2019 The Qt Company.");
	connect(ui->action_About,  &QAction::triggered,   aboutBox, &QMessageBox::exec);
	connect(ui->button_AddRow, &QPushButton::clicked, this,     &ProfileWindow::addRowEmpty);
	connect(ui->button_Save,   &QPushButton::clicked, this,     &ProfileWindow::saveProfiles);
}

ProfileWindow::~ProfileWindow() {}

void ProfileWindow::closeEvent(QCloseEvent* event) {
	if (unsavedChanges) {
		QMessageBox unsavedPrompt(
			QMessageBox::Question,
			"SDM: Unsaved Changes",
			"There are unsaved changes to the profiles.\n\nWhat would you like to do with them?",
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
			this,
			Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint
		);
		unsavedPrompt.setDefaultButton(QMessageBox::Save);
		int response = unsavedPrompt.exec();
		switch (response) {
			case QMessageBox::Save:
				saveProfiles();
				event->accept();
				break;
			case QMessageBox::Discard:
				event->accept();
				break;
			case QMessageBox::Cancel:
				event->ignore();
				break;
			default:
				break;
		}
	}
	else {
		event->accept();
	}
}

void ProfileWindow::addRow(const bool active, const QString url, const QString element) {
	madeChange();
	int gridRowCount = ui->layout_Config_G->rowCount();
	QCheckBox* rowActive  = new QCheckBox(this);
	QLineEdit* rowUrl     = new QLineEdit(url,     this);
	QLineEdit* rowElement = new QLineEdit(element, this);
	rowActive->setChecked(active);
	connect(rowActive,  &QCheckBox::toggled,     this, &ProfileWindow::madeChange);
	connect(rowUrl,     &QLineEdit::textChanged, this, &ProfileWindow::madeChange);
	connect(rowElement, &QLineEdit::textChanged, this, &ProfileWindow::madeChange);
	ui->layout_Config_G->addWidget(rowActive,  gridRowCount, 0, 1, 1, Qt::AlignRight);
	ui->layout_Config_G->addWidget(rowUrl,     gridRowCount, 1);
	ui->layout_Config_G->addWidget(rowElement, gridRowCount, 2);
}

//void ProfileWindow::removeRow(const unsigned i) {
	// TODO: Implement
	// Remove ith row of internal 2D QLayoutItem pointer array
	// Call method that deletes and replaces existing layout
//}

void ProfileWindow::loadProfiles() {
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
	QActionGroup* profileActions = new QActionGroup(ui->menu_Profiles);
	profileActions->setExclusive(true);
	for (auto it = profiles.constBegin(); it != profiles.constEnd(); ++it) {
		QString     profileName   = it.key();
		QJsonObject profileData   = it.value().toObject();
		QAction*    profileAction = new QAction(profileName, ui->menu_Profiles);
		profileAction->setCheckable(true);
		profileActions->addAction(profileAction);
		ui->menu_Profiles->insertAction(ui->action_New, profileAction);
		if (profileData.value("current").toBool()) {
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
	unsavedChanges = false;
}

void ProfileWindow::madeChange() {
	unsavedChanges = true;
}

void ProfileWindow::saveProfiles() {
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
	unsavedChanges = false;
}

void ProfileWindow::addRowEmpty() {
	addRow(false, "", "");
}
