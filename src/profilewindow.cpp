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

namespace SDM {
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
		connect(ui->action_About,        &QAction::triggered,   aboutBox, &QMessageBox::exec);
		connect(ui->button_AddTarget,    &QPushButton::clicked, this,     &ProfileWindow::addTargetBlank);
		connect(ui->button_RemoveTarget, &QPushButton::clicked, this,     &ProfileWindow::removalMenu);
		connect(ui->button_Save,         &QPushButton::clicked, this,     &ProfileWindow::saveProfiles);
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

	void ProfileWindow::shrinkToFit() {
		centralWidget()->adjustSize();
		adjustSize();
	}

	void ProfileWindow::setActiveProfile(QAction* action) {
		clearTargets();
		currentProfileName = action->text();
		QJsonObject profileData = profiles.value(currentProfileName).toObject();
		int profileInterval = profileData.value("interval").toInt();
		ui->spinBox_Hours->setValue(profileInterval / 60);
		ui->spinBox_Minutes->setValue(profileInterval % 60);
		QJsonArray targets = profileData.value("targets").toArray();
		for (int i = 0; i < targets.size(); ++i) {
			QJsonObject target = targets[i].toObject();
			addTarget(
				target.value("active").toBool(),
				target.value("url").toString(),
				target.value("element").toString()
			);
		}
		shrinkToFit();
		madeChange();
	}

	void ProfileWindow::addTarget(const bool active, const QString& url, const QString& element) {
		QCheckBox* tarActive = new QCheckBox(this);
		QLineEdit* tarURL    = new QLineEdit(url, this);
		QLineEdit* tarElem   = new QLineEdit(element, this);
		tarActive->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
		tarURL->setSizePolicy(QSizePolicy::Expanding,    QSizePolicy::MinimumExpanding);
		tarElem->setSizePolicy(QSizePolicy::Expanding,   QSizePolicy::MinimumExpanding);
		tarActive->setChecked(active);
		connect(tarActive,  &QCheckBox::toggled,     this, &ProfileWindow::madeChange);
		connect(tarURL,     &QLineEdit::textChanged, this, &ProfileWindow::madeChange);
		connect(tarElem,    &QLineEdit::textChanged, this, &ProfileWindow::madeChange);
		ui->layout_Active_V->addWidget(tarActive, 0, Qt::AlignRight);
		ui->layout_URL_V->addWidget(tarURL);
		ui->layout_Element_V->addWidget(tarElem);
		madeChange();
	}

	void ProfileWindow::removalMenu() {
		QMenu* menu = new QMenu(this);
		menu->setAttribute(Qt::WA_DeleteOnClose, true);
		for (int i = 0; i < ui->layout_URL_V->count(); ++i) {
			QString urlString  = qobject_cast<QLineEdit*>(ui->layout_URL_V->itemAt(i)->widget())->text();
			QString elemString = qobject_cast<QLineEdit*>(ui->layout_Element_V->itemAt(i)->widget())->text();
			if (urlString.isEmpty())  urlString  = "Blank";
			if (elemString.isEmpty()) elemString = "Blank";
			QAction* target = new QAction(QString("%1::%2").arg(urlString, elemString));
			target->setIconText(QString::number(i));
			menu->addAction(target);
		}
		connect(menu, &QMenu::triggered, this, &ProfileWindow::removeTarget);
		menu->exec(QCursor::pos());
	}

	void ProfileWindow::removeTarget(QAction* target) {
		int row = target->iconText().toInt();
		QWidget* tarActive = ui->layout_Active_V->itemAt(row)->widget();
		QWidget* tarURL    = ui->layout_URL_V->itemAt(row)->widget();
		QWidget* tarElem   = ui->layout_Element_V->itemAt(row)->widget();
		ui->layout_Active_V->removeWidget(tarActive);
		ui->layout_URL_V->removeWidget(tarURL);
		ui->layout_Element_V->removeWidget(tarElem);
		tarActive->deleteLater();
		tarURL->deleteLater();
		tarElem->deleteLater();
		shrinkToFit();
		madeChange();
	}

	void ProfileWindow::clearTargets() {
		while (ui->layout_Active_V->count() > 0) {
			QWidget* tarActive = ui->layout_Active_V->itemAt(0)->widget();
			ui->layout_Active_V->removeWidget(tarActive);
			tarActive->deleteLater();
		}
		while (ui->layout_URL_V->count() > 0) {
			QWidget* tarURL = ui->layout_URL_V->itemAt(0)->widget();
			ui->layout_URL_V->removeWidget(tarURL);
			tarURL->deleteLater();
		}
		while (ui->layout_Element_V->count() > 0) {
			QWidget* tarElem = ui->layout_Element_V->itemAt(0)->widget();
			ui->layout_Element_V->removeWidget(tarElem);
			tarElem->deleteLater();
		}
	}

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
		connect(profileActions, &QActionGroup::triggered, this, &ProfileWindow::setActiveProfile);
		for (auto it = profiles.constBegin(); it != profiles.constEnd(); ++it) {
			QString     profileName   = it.key();
			QJsonObject profileData   = it.value().toObject();
			QAction*    profileAction = new QAction(profileName, ui->menu_Profiles);
			profileAction->setCheckable(true);
			profileActions->addAction(profileAction);
			ui->menu_Profiles->insertAction(ui->action_NewProfile, profileAction);
			if (profileData.value("current").toBool()) {
				profileAction->setChecked(true);
				int profileInterval = profileData.value("interval").toInt();
				ui->spinBox_Hours->setValue(profileInterval / 60);
				ui->spinBox_Minutes->setValue(profileInterval % 60);
				currentProfileName = profileName;
				QJsonArray targets = profileData.value("targets").toArray();
				for (int i = 0; i < targets.size(); ++i) {
					QJsonObject target = targets[i].toObject();
					addTarget(
						target.value("active").toBool(),
						target.value("url").toString(),
						target.value("element").toString()
					);
				}
			}
		}
		ui->menu_Profiles->insertSeparator(ui->action_NewProfile);
		unsavedChanges = false;
	}

	void ProfileWindow::madeChange() {
		unsavedChanges = true;
	}

	void ProfileWindow::saveProfiles() {
		QJsonObject profile;
		profile.insert("current", true);
		int interval = (ui->spinBox_Hours->value() * 60) + ui->spinBox_Minutes->value();
		if (interval < 10) {
			interval = 10; // Reasonable minimum of 10 minutes between updates for now.
		}
		profile.insert("interval", interval);
		QJsonArray targets;
		for (int i = 0; i < ui->layout_URL_V->count(); ++i) {
			bool    rowActive  = qobject_cast<QCheckBox*>(ui->layout_Active_V->itemAt(i)->widget())->isChecked();
			QString rowURL     = qobject_cast<QLineEdit*>(ui->layout_URL_V->itemAt(i)->widget())->text();
			QString rowElement = qobject_cast<QLineEdit*>(ui->layout_Element_V->itemAt(i)->widget())->text();
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

	void ProfileWindow::addTargetBlank() {
		addTarget(false, "", "");
	}
}
