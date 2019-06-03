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

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	aboutBox(new QMessageBox(this)) {
	ui->setupUi(this);
	ui->layout_Config_G->addWidget(new QCheckBox(), 1, 0, 1, 1, Qt::AlignRight);
	ui->layout_Config_G->addWidget(new QLineEdit(), 1, 1);
	ui->layout_Config_G->addWidget(new QLineEdit(), 1, 2);
	ui->layout_Config_G->addWidget(new QCheckBox(), 2, 0, 1, 1, Qt::AlignRight);
	ui->layout_Config_G->addWidget(new QLineEdit(), 2, 1);
	ui->layout_Config_G->addWidget(new QLineEdit(), 2, 2);
	ui->layout_Config_G->addWidget(new QCheckBox(), 3, 0, 1, 1, Qt::AlignRight);
	ui->layout_Config_G->addWidget(new QLineEdit(), 3, 1);
	ui->layout_Config_G->addWidget(new QLineEdit(), 3, 2);
	ui->layout_Config_G->addWidget(new QCheckBox(), 4, 0, 1, 1, Qt::AlignRight);
	ui->layout_Config_G->addWidget(new QLineEdit(), 4, 1);
	ui->layout_Config_G->addWidget(new QLineEdit(), 4, 2);
	ui->layout_Config_G->addWidget(new QCheckBox(), 5, 0, 1, 1, Qt::AlignRight);
	ui->layout_Config_G->addWidget(new QLineEdit(), 5, 1);
	ui->layout_Config_G->addWidget(new QLineEdit(), 5, 2);

	aboutBox->setWindowTitle("SDM: About");
	aboutBox->setText("This project was created by github.com/dGrowl.");
	aboutBox->setInformativeText("Created using Qt.\nQt is Copyright © 2019 The Qt Company.");
	connect(ui->action_About, &QAction::triggered, aboutBox, &QMessageBox::exec);
}

MainWindow::~MainWindow() {
	delete ui;
}
