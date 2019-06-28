#include "inc/reportwindow.h"
#include "ui_reportwindow.h"

namespace SDM {
	ReportWindow::ReportWindow(const QString url, const QString previous, const QString current, QWidget* parent):
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
			connect(ui->button_Close, &QPushButton::pressed, this, &ReportWindow::deleteLater);
		}
		catch (std::exception& e) {
			qDebug() << e.what() << "Error: Failed to construct SDM::ReportWindow.";
			deleteLater();
		}
	}

	ReportWindow::~ReportWindow() {}
}
