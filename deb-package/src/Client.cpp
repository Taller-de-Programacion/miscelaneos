#include "Client.h"
#include "ui_Client.h"
#include <QMessageBox>
#include <QAbstractSocket>

Client::Client(QWidget *parent) : QWidget(parent), tcpSocket(this) {
    Ui::Client clientUi;
    clientUi.setupUi(this);
    connectEvents();
}

void Client::connectToServer() {
    QPushButton* buttonConnect = findChild<QPushButton*>("buttonConnect");
    buttonConnect->setEnabled(false);

    QLineEdit* inputHost = findChild<QLineEdit*>("inputHost");
    QLineEdit* inputPort = findChild<QLineEdit*>("inputPort");
    QLabel* labelOut = findChild<QLabel*>("labelOut");
    QString host = inputHost->text();
    QString port = inputPort->text();
    QString status = QString("Connect: %1:%2").arg(host, port);
    labelOut->setText(status);
    // Si había otra conexión, la cierro, sino, no hace nada
    tcpSocket.abort();
    tcpSocket.connectToHost(host, port.toInt());
    labelOut->setText("Connected!");
}

void Client::notifyError(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Hello client"),
                                 tr("El host no fue encontrado"));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Hello client"),
                                 tr("La conexión fue rechazada"));
        break;
    default:
        QMessageBox::information(this, tr("Hello client"),
                                 tr("Error: %1.")
                                 .arg(tcpSocket.errorString()));
    }

    QPushButton* buttonConnect = findChild<QPushButton*>("buttonConnect");
    buttonConnect->setEnabled(true);
}

void Client::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonConnect = findChild<QPushButton*>("buttonConnect");
    QObject::connect(buttonConnect, &QPushButton::clicked,
                     this, &Client::connectToServer);
    // Eventos de socket
    QObject::connect(&this->tcpSocket, &QIODevice::readyRead, this, &Client::readHello);
    // Magia oscura para usar la señal: https://stackoverflow.com/questions/61677080/error-qabstractsocket-while-installing-mqtt-in-qt-c
    QObject::connect(&this->tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
        this, &Client::notifyError);
}

void Client::readHello() {
    QByteArray bytes = this->tcpSocket.readAll();
    // Super inseguro, quizá no me llegaron todos los bytes de "hello"
    QString msg(bytes);
    QLabel* labelOut = findChild<QLabel*>("labelOut");
    labelOut->setText(msg);

    QPushButton* buttonConnect = findChild<QPushButton*>("buttonConnect");
    buttonConnect->setEnabled(true);
}