#include "createuser.h"
#include "ui_createuser.h"
#include <qthread.h>
#include <qmessagebox.h>
#include <regex>
#include <string>
#include "login.h"

CreateUser::CreateUser(chatClient* cl, Login* lo, QWidget *parent):
    QWidget(parent),
    ui(new Ui::CreateUser),
	window_to_call(lo),
	cl(cl)
{
	setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
	setFixedSize(664, 369);
    ui->setupUi(this);

	connect(cl, SIGNAL(createSuccess()), this, SLOT(create_success()));
	connect(cl, SIGNAL(createNameExisted()), this, SLOT(create_name_existed()));
}

CreateUser::~CreateUser()
{
    delete ui;
}

void CreateUser::on_Create_Button_released() {
	auto username = ui->Username->text(),
		password = ui->Password->text(),
		name = ui->Name->text();

	if (username.isEmpty() || password.isEmpty() || name.isEmpty()) {
		ui->Create_Label->setText("�û�����������ǳƲ���Ϊ�գ�");
		return;
	}

	std::regex r("^[0-9a-zA-Z_]{3,15}$");
	std::string pwd = password.toStdString();
	if (!std::regex_match(pwd, r)) {
		ui->Create_Label->setText("����Ӧ�������֡�Ӣ����ĸ���»�����ɣ��ҳ���ӦΪ3-15֮�䡣");
		return;
	}

	cl->sendMessage("~@Client:Create");
	cl->sendMessage(username);
	QThread::msleep(100);
	cl->sendMessage(password);
	QThread::msleep(100);
	cl->sendMessage(name);
}

void CreateUser::create_success() {
	//��¼


	QMessageBox msg(this);
	msg.setWindowTitle("���ע��");
	msg.setText("��ϲ�����ע�ᣬ�������������¼���棬���������˺ź��������������ҡ�");
	msg.setIcon(QMessageBox::Information);
	msg.setStandardButtons(QMessageBox::Ok);
	msg.exec();

	window_to_call->show();
	this->close();
}

void CreateUser::create_name_existed() {
	ui->Create_Label->setText("�û����Ѿ���ʹ�ã������һ��");
}