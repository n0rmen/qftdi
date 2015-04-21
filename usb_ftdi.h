#ifndef NEW_USB_H
#define NEW_USB_H

#include <QThread>
#include <windows.h>
#include "ftd2xx.h"
#include <stdint.h>

class USB_FTDI : public QThread
{
    Q_OBJECT

public:
    USB_FTDI(bool need_thread = true, unsigned long ms = 100);
    ~USB_FTDI();

    bool isOpen(void);

    unsigned long getVersion(void);
    unsigned long listDevices(void);
    bool open(int devNum = 0, ULONG baudRate = 3000000);
    unsigned long close();
    QByteArray *read(unsigned long RxBytes = 0);//< если указатель на буфер пустой, то создается динамически!
    unsigned long write(QByteArray *data, unsigned long byteWrite = 0);
    unsigned long getStatusBuffer();

    void run();
    virtual void threadCycle();
    void startThread();

signals:
    void receive();

private:
    FT_HANDLE handle;
    bool thread_on;
    unsigned long msecs;
};

#endif // NEW_USB_H
