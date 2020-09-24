#include <QtWidgets/QMainWindow>
#include "Client.h"
#include "NameAccepter.h"
#include "ui_MainWindow.h"
#include "ConversationDialog.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	void ShowNameAccepter();

public slots:
	void CloseApplication();
	void GetName(QString name);
	void AppendNewHostToList(QString host);

private slots:
	void OnTalkButtonClick();

private:
	Ui::MainWindowClass ui;
	Client client;
	NameAccepter nameAccepterDialog;
	ConversationDialog conversationDialog;
};
