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
 * GNU Leser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SDM. If not, see <https://www.gnu.org/licenses/>.
 */

#include "inc/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	ui->gridLayout->addWidget(new QCheckBox(), 1, 0, 1, 1, Qt::AlignRight);
	ui->gridLayout->addWidget(new QLineEdit(), 1, 1);
	ui->gridLayout->addWidget(new QLineEdit(), 1, 2);
	ui->gridLayout->addWidget(new QCheckBox(), 2, 0, 1, 1, Qt::AlignRight);
	ui->gridLayout->addWidget(new QLineEdit(), 2, 1);
	ui->gridLayout->addWidget(new QLineEdit(), 2, 2);
	ui->gridLayout->addWidget(new QCheckBox(), 3, 0, 1, 1, Qt::AlignRight);
	ui->gridLayout->addWidget(new QLineEdit(), 3, 1);
	ui->gridLayout->addWidget(new QLineEdit(), 3, 2);
	ui->gridLayout->addWidget(new QCheckBox(), 4, 0, 1, 1, Qt::AlignRight);
	ui->gridLayout->addWidget(new QLineEdit(), 4, 1);
	ui->gridLayout->addWidget(new QLineEdit(), 4, 2);
	ui->gridLayout->addWidget(new QCheckBox(), 5, 0, 1, 1, Qt::AlignRight);
	ui->gridLayout->addWidget(new QLineEdit(), 5, 1);
	ui->gridLayout->addWidget(new QLineEdit(), 5, 2);
}

MainWindow::~MainWindow() {
	delete ui;
}
