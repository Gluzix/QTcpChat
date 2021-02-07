#include "MainWindow.h"
#include <QFont>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(&client, SIGNAL(PassDataToConversation(QString, QString)), this, SLOT(GetDataAndId(QString, QString)));
	connect(&client, SIGNAL(PassIdToHostList(QString)), this, SLOT(AppendNewHostToList(QString)));
	connect(this, SIGNAL(PassIdToSend(QString)), &client, SLOT(GetIdToSend(QString)));
	connect(&client, SIGNAL(SendIdToRemove(QString)), this, SLOT(RemoveId(QString)));
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(OnConfirmButtonClick()));
	connect(ui.connectedListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OpenDialog(QListWidgetItem*)));

	int id = QFontDatabase::addApplicationFont(":/fonts/Open_Sans/OpenSans-Regular.ttf");
	QFont font("Open Sans", 12);
	qApp->setFont(font);
}

MainWindow::~MainWindow()
{
	qDeleteAll(dialogVector.begin(), dialogVector.end());
}

void MainWindow::OnConfirmButtonClick()
{
	QString name = ui.nameEdit->text();
	if (!name.isEmpty())
	{
		client.SetUserName(name);
		ui.infoLabel->setText("");
		ui.connectedListWidget->setDisabled(false);
		ui.okButton->setDisabled(true);
		ui.nameEdit->setDisabled(true);
	}
	else
	{
		ui.infoLabel->setText("Please don't set empty name");
	}
}

void MainWindow::AppendNewHostToList(QString host)
{
	ui.connectedListWidget->addItem(host);
}

void MainWindow::GetDataAndId(QString data, QString id)
{
	bool bIfFound = false;
	for (QVector<ConversationDialog*>::iterator it = dialogVector.begin(); it < dialogVector.end(); ++it)
	{
		if ((*it)->GetId() == id)
		{
			bIfFound = true;
			(*it)->SetData(data);
		}
	}

	if (!bIfFound)
	{
		ConversationDialog *dialog = new ConversationDialog();
		connect(dialog, SIGNAL(PassDataToSend(QString, QString)), &client, SLOT(GetMessage(QString, QString)));
		dialog->SetConversationId(id);
		dialog->SetData(data);
		dialogVector.push_back(dialog);
	}
}

void MainWindow::RemoveId(QString id)
{
	for (int i = 0; i < ui.connectedListWidget->count(); ++i)
	{
		QStringList list = ui.connectedListWidget->item(i)->text().split(QRegExp("[(,)]"));

		if (list[1].trimmed() == id)
		{
			QListWidgetItem* item = ui.connectedListWidget->item(i);
			delete item;
			break;
		}
	}

	for (QVector<ConversationDialog*>::iterator it = dialogVector.begin(); it < dialogVector.end(); ++it)
	{
		if ((*it)->GetId() == id)
		{
			delete (*it);
			dialogVector.removeOne((*it));
		}
	}
}

void MainWindow::OpenDialog(QListWidgetItem* item)
{
	QString content = item->text();
	QStringList id = content.split(QRegExp("[(,)]"));
	emit PassIdToSend(id[1]);
	bool bIfFound = false;
	for (QVector<ConversationDialog*>::iterator it = dialogVector.begin(); it < dialogVector.end(); ++it)
	{
		if ((*it)->GetId() == id[1])
		{			
			bIfFound = true;
			(*it)->show();
			break;
		}
	}

	if (!bIfFound)
	{
		ConversationDialog *dialog = new ConversationDialog();
		connect(dialog, SIGNAL(PassDataToSend(QString, QString)), &client, SLOT(GetMessage(QString, QString)));
		dialog->SetConversationId(id[1]);
		dialog->show();
		dialogVector.push_back(dialog);
	}
}