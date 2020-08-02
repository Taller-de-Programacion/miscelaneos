#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

class Client : public QWidget {
public:
    explicit Client(QWidget *parent = 0);
private:
    QTcpSocket tcpSocket;
    /**
     * Conectarse usando un socket tcp
     **/
    void connectToServer();
    /**
     * Conectar eventos de GUI
     **/
    void connectEvents();
    /**
     * Recibir un "hello" desde el servidor
     **/
    void readHello();
    /**
     * Imprimir errores de conexión
     */
    void notifyError(QAbstractSocket::SocketError socketError);
};

#endif // CLIENT_H
