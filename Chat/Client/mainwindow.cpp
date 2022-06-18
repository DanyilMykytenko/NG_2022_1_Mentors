#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_socket = new QTcpSocket();
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabText(0, "Login");
    ui->tabWidget->setTabText(1, "Chat");

    ui->l_ip->setText("127.0.0.1");
    ui->sb_port->setValue(5901);

    connect(m_socket, &QTcpSocket::connected, this, &MainWindow::connected);
    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::dataReceived);

    connect(ui->b_connect, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(ui->b_send, &QPushButton::clicked, this, &MainWindow::sendMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToServer()
{
    m_socket->connectToHost(ui->l_ip->text(), ui->sb_port->value());
}

void MainWindow::connected()
{
    ui->tabWidget->setCurrentIndex(1);
    m_socket->write("<n>" + ui->l_nickname->text().toUtf8());
}

void MainWindow::sendMessage()
{
    QByteArray data = ui->p_message->toPlainText().toUtf8();
    if(data.indexOf("\n") != -1)
    {
        data.resize(data.size()-1);
    }
    qDebug() << data.indexOf("\n");
    m_socket->write("<m>" + data);
    ui->p_message->clear();
}

void MainWindow::dataReceived()
{
    ui->p_chat->setPlainText(ui->p_chat->toPlainText() + m_socket->readAll() + "\n");
}

