#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.talkButton, SIGNAL(clicked()), this, SLOT(OnTalkButtonClick()));
	connect(&client, SIGNAL(PassDataToConversation(QString)), &conversationDialog, SLOT(GetData(QString)));
	connect(&conversationDialog, SIGNAL(PassDataToSend(QString, QString)), &client, SLOT(GetMessage(QString, QString)));

	connect(&nameAccepterDialog, SIGNAL(SendExit()), this, SLOT(CloseApplication()));
	connect(&nameAccepterDialog, SIGNAL(SendName(QString)), this, SLOT(GetName(QString)));

	connect(&client, SIGNAL(PassIdToHostList(QString)), this, SLOT(AppendNewHostToList(QString)));

	connect(this, SIGNAL(PassIdToSend(QString)), &client, SLOT(GetIdToSend(QString)));
}

void MainWindow::ShowNameAccepter()
{
	this->setEnabled(false);
	nameAccepterDialog.show();
}

void MainWindow::OnTalkButtonClick()
{
	QString text = ui.connectedListWidget->currentItem()->text();
	QStringList id = text.split(QRegExp("[(,)]"));
	emit PassIdToSend(id[1]);
	conversationDialog.SetConversationId(id[1]);
	conversationDialog.show();
}

void MainWindow::CloseApplication()
{
	nameAccepterDialog.close();
	this->setEnabled(true);
	QMainWindow::close();
	QApplication::quit();
}

void MainWindow::GetName(QString name)
{
	ui.nameLabel->setText("Name: "+name);
	client.SetUserName(name);
	nameAccepterDialog.close();
	this->setEnabled(true);
}

void MainWindow::AppendNewHostToList(QString host)
{
	ui.connectedListWidget->addItem(host);
}