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
		explicit ReportWindow(const QString url, const QString previous, const QString current, QWidget* parent = nullptr);
		virtual ~ReportWindow();

	private:
		std::unique_ptr<Ui::ReportWindow> ui;
	};
}

#endif // REPORTWINDOW_H
