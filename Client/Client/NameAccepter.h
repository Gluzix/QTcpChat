#include <QDialog>
#include "ui_NameAccepter.h"

class NameAccepter : public QDialog
{
	Q_OBJECT

public:
	NameAccepter(QDialog *parent=Q_NULLPTR);
	~NameAccepter();

private slots:
	void OnNameEditChange(QString text);
	void OnOkButtonClick();
	void OnCancelButtonClick();

signals:
	void SendName(QString Name);
	void SendExit();

private:
	Ui::Dialog ui;
};
