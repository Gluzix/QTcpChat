#include "ConversationDialog.h"

ConversationDialog::ConversationDialog(QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

ConversationDialog::~ConversationDialog()
{
}

void ConversationDialog::GetData(QString data)
{
	ui.incomingEdit->append(QString(data));
}

void ConversationDialog::OnSendButtonClick()
{
	QString message = ui.outcomingEdit->toPlainText();
	ui.outcomingEdit->clear();
	ui.incomingEdit->append(QString("You: " + message));
	emit PassDataToSend(message);
}