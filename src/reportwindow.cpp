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

#include "inc/reportwindow.h"
#include "ui_reportwindow.h"

namespace SDM {
	ReportWindow::ReportWindow(const QString& url, const QString& previous, const QString& current, QWidget* parent):
		QWidget(parent),
		ui(nullptr) {
		setAttribute(Qt::WA_QuitOnClose, false);
		try {
			ui = std::make_unique<Ui::ReportWindow>();
			ui->setupUi(this);
			setWindowIcon(QIcon(":/res/icon.png"));
			setAttribute(Qt::WA_DeleteOnClose, true);
			ui->lineEdit_URL->setText(url);
			ui->lineEdit_URL->setReadOnly(true);
			ui->textEdit_Previous->insertPlainText(previous);
			ui->textEdit_Current->insertPlainText(current);
			connect(ui->button_Close, &QPushButton::clicked, this, &ReportWindow::deleteLater);
		}
		catch (std::exception& e) {
			qDebug() << e.what() << "Error: Failed to construct SDM::ReportWindow.";
			deleteLater();
		}
	}

	ReportWindow::~ReportWindow() {}
}
