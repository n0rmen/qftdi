#include "qftdi.h"

#include <QDebug>
/*
WorkThread::WorkThread(QObject *parent) : QThread(parent)
{

}
*/
QFTDI::QFTDI(QObject *parent, qint64 numdev, qint32 baudrate, qint64 thread_timeout) :
    QIODevice(parent)
{
    handle = NULL;
    bitmode = BITMODE_FAST_SERIAL;
    word_length = BITS_8;
    stop_bits = STOP_BITS_1;
    parity = PARIT_NONE;
    num_dev = numdev;
    baud_rate = baudrate;
    timeout = thread_timeout;

    thread = new WorkThread(this);
}

QFTDI::~QFTDI()
{
    if (handle != NULL) close();
}

quint8 QFTDI::listDevices()
{
    DWORD number;
    if (FT_CreateDeviceInfoList(&number) == FT_OK) {
        return number;
    }
    return 0;
}

quint32 QFTDI::getVersion()
{
    DWORD version = 0;
    FT_GetLibraryVersion(&version);
    return version;
}

bool QFTDI::open(OpenMode mode)
{
    if (mode == ReadWrite) {
        if ( (FT_Open(num_dev, &handle) == FT_OK) &&
             (FT_SetBitMode(handle, 0x0B, bitmode) == FT_OK) &&
             (FT_SetDataCharacteristics(handle, word_length, stop_bits, parity) == FT_OK) &&
             (FT_SetBaudRate(handle, baud_rate) == FT_OK)) {
            FT_Purge(handle, FT_PURGE_RX | FT_PURGE_TX);
            if (timeout) thread->start();
            return QIODevice::open(mode);
        }
    }
    return false;
}

bool QFTDI::open(qint32 numdev, OpenMode mode, qint32 baudrate)
{
    num_dev = numdev;
    if (baudrate) baud_rate = baudrate;
    return open(mode);
}

void QFTDI::close()
{
    if (handle != NULL) FT_Close(handle);
    handle = NULL;
    QIODevice::close();
}

qint64 QFTDI::bytesAvailable() const
{
    DWORD rx_byte, tx_byte, event;
    if (FT_GetStatus(handle, &rx_byte, &tx_byte, &event) == FT_OK) {
        return rx_byte;
    }
    return 0;
}

qint64 QFTDI::readData(char *data, qint64 maxlen)
{
    if (handle != NULL) {
        DWORD bytes = bytesAvailable();
        if (maxlen > bytes) maxlen = bytes;
        bytes = 0;
        FT_Read(handle, data, maxlen, &bytes);
        return bytes;
    }
    return 0;
}

qint64 QFTDI::writeData(const char *data, qint64 len)
{
    DWORD bytes = 0;
    if (handle != NULL) FT_Write(handle, (LPVOID)data, len, &bytes);
    return bytes;
}
