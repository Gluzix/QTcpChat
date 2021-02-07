#include <QtWidgets/QMainWindow>
#include "Client.h"
#include "ui_MainWindow.h"
#include "ConversationDialog.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

signals:
	void PassIdToSend(QString id);

public slots:
	void AppendNewHostToList(QString host);
	void GetDataAndId(QString data, QString id);
	void RemoveId(QString id);
	void OpenDialog(QListWidgetItem *item);

private slots:
	void OnConfirmButtonClick();

private:
	Ui::MainWindowClass ui;
	Client client;
	QVector<ConversationDialog*> dialogVector;
};
