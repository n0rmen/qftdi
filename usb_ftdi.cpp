#include "usb_ftdi.h"

#include <QDebug>

USB_FTDI::USB_FTDI(bool need_thread, unsigned long ms)
{
    handle = NULL;

    if (need_thread) {
        thread_on = true;
        msecs = ms;
        this->start();
    }

}

USB_FTDI::~USB_FTDI()
{
    thread_on = false;
    close();
    quit();
    wait();
    qDebug() << "End Distr";
}

void USB_FTDI::run()
{
    while(thread_on) {
        if (isOpen()) {
            if (getStatusBuffer()) emit receive();
        }
        msleep(msecs);
    }
    qDebug() << "End Thread!";
}

bool USB_FTDI::isOpen(void)
{
    if (handle == NULL) return false;
    else return true;
}

unsigned long USB_FTDI::getVersion()
{
    DWORD version = 0;
    FT_GetLibraryVersion(&version);
    return version;
}

unsigned long USB_FTDI::listDevices()
{
    DWORD numDevs;
    if (FT_CreateDeviceInfoList(&numDevs) == FT_OK) {
        return numDevs;
    }
    return 0;
}

bool USB_FTDI::open(int devNum, ULONG baudRate)
{
    if(FT_Open(devNum, &handle) == FT_OK
            && (FT_SetBitMode(handle, 0x0B, FT_BITMODE_FAST_SERIAL) == FT_OK)
            && (FT_SetDataCharacteristics(handle, FT_BITS_8,  FT_STOP_BITS_1,  FT_PARITY_NONE) == FT_OK)
            && (FT_SetBaudRate(handle, baudRate) == FT_OK)) {
            FT_Purge(handle, FT_PURGE_RX | FT_PURGE_TX);
            return true;
    }
    return false;
}

unsigned long USB_FTDI::close()
{
    unsigned long res = FT_Close(handle);
    handle = NULL;
    return res;
}

QByteArray* USB_FTDI::read(unsigned long byteRead)
{
    DWORD BytesReceived;

    if (byteRead == 0) byteRead = getStatusBuffer();

     char * RxBuffer = new char[byteRead+1];

    if (FT_Read(handle, RxBuffer, byteRead, &BytesReceived) != FT_OK) {
        delete [] RxBuffer;
        return NULL;
    }
    QByteArray * data = new QByteArray(RxBuffer, BytesReceived);
    return data;
}

unsigned long USB_FTDI::getStatusBuffer()
{
    DWORD RxBytes, TxBytes, EventDWord;

    if (FT_GetStatus(handle,&RxBytes,&TxBytes,&EventDWord) != FT_OK)
        return 0;
    return RxBytes;
}

unsigned long USB_FTDI::write(QByteArray *data, unsigned long byteWrite)
{
    DWORD BytesWritten;

    if (byteWrite == 0) byteWrite = data->size();

    if (FT_Write(handle, (LPVOID)data->constData(), byteWrite, &BytesWritten) != FT_OK) return 0;
    //else FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);

    return BytesWritten;
}
