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

#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include <memory>
#include <QDebug>
#include <QWidget>

namespace Ui {
	class ReportWindow;
}

namespace SDM {
	class ReportWindow: public QWidget {
		Q_OBJECT

	public:
		explicit ReportWindow(const QString& url, const QString& previous, const QString& current, QWidget* parent = nullptr);
		virtual ~ReportWindow();

	private:
		std::unique_ptr<Ui::ReportWindow> ui;
	};
}

#endif // REPORTWINDOW_H
