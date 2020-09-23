#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.talkButton, SIGNAL(clicked()), this, SLOT(OnTalkButtonClick()));
	connect(&client, SIGNAL(PassDataToConversation(QString)), &conversationDialog, SLOT(GetData(QString)));
	connect(&conversationDialog, SIGNAL(PassDataToSend(QString)), &client, SLOT(GetMessage(QString)));

	connect(&nameAccepterDialog, SIGNAL(SendExit()), this, SLOT(CloseApplication()));
	connect(&nameAccepterDialog, SIGNAL(SendName(QString)), this, SLOT(GetName(QString)));

	connect(&client, SIGNAL(PassIdToHostList(QString)), this, SLOT(AppendNewHostToList(QString)));
}

void MainWindow::ShowNameAccepter()
{
	this->setEnabled(false);
	nameAccepterDialog.show();
}

void MainWindow::OnTalkButtonClick()
{

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
	client.SetUserName(name);
	nameAccepterDialog.close();
	this->setEnabled(true);
}

void MainWindow::AppendNewHostToList(QString host)
{
	ui.connectedListWidget->addItem(host);
}