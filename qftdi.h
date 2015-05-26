#ifndef QFTDI_H
#define QFTDI_H

#include <QIODevice>
#include <QThread>
#include <windows.h>
#include "ftd2xx.h"

#define STANDART_BAUTRATE   57600

class QFTDI : public QIODevice
{
public:
    QFTDI(QObject *parent = 0, qint64 numdev = 0, qint32 baudrate = STANDART_BAUTRATE, qint64 thread_timeout = 0);
    ~QFTDI();

    static quint8 listDevices();
    static quint32 getVersion();

    bool isSequential() {return true;}
    virtual bool open(OpenMode mode);
    bool open(qint32 numdev, OpenMode mode, qint32 baudrate = 0);
    virtual void close();

    qint64 bytesAvailable() const;

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);


    typedef enum {
        BITMODE_RESET =         FT_BITMODE_RESET,
        BITMODE_ASYNC_BITBANG = FT_BITMODE_ASYNC_BITBANG,
        BITMODE_MPSSE =         FT_BITMODE_MPSSE,
        BITMODE_SYNC_BITBANG =  FT_BITMODE_SYNC_BITBANG,
        BITMODE_MCU_HOST =      FT_BITMODE_MCU_HOST,
        BITMODE_FAST_SERIAL =   FT_BITMODE_FAST_SERIAL,
        BITMODE_CBUS_BITBANG =  FT_BITMODE_CBUS_BITBANG,
        BITMODE_SYNC_FIFO =     FT_BITMODE_SYNC_FIFO
    } BitMode;
    void setBitMode(BitMode mode) { bitmode = mode;}
    BitMode getBitMode() const { return (BitMode)bitmode;}

    typedef enum {
        BITS_8 = FT_BITS_8,
        BITS_7 = FT_BITS_7
    } DataLength;
    void setDataLength(DataLength length) { word_length = length;}
    DataLength getDataLength() const { return (DataLength)word_length;}

    typedef enum {
        STOP_BITS_1 = FT_STOP_BITS_1,
        STOP_BITS_2 = FT_STOP_BITS_2
    } StopBits;
    void setStopBits(StopBits stop) { stop_bits = stop;}
    StopBits getStopBits() const { return (StopBits)stop_bits;}

    typedef enum {
        PARIT_NONE = FT_PARITY_NONE,
        PARIT_ODD = FT_PARITY_ODD,
        PARIT_EVEN = FT_PARITY_EVEN,
        PARIT_MARK = FT_PARITY_MARK,
        PARIT_SPACE = FT_PARITY_SPACE
    }Parity;
    void setParity(Parity par) { parity = par;}
    Parity getParity() const { return (Parity)parity;}

    void setBaudRate(qint32 baud) { if (baud <= 3000000) baud_rate = baud; else baud_rate = STANDART_BAUTRATE;}
    qint32 getBaudRate() const { return baud_rate;}

    void setNumDev(qint64 num) { num_dev = num;}
    qint64 getNumDev() const { return num_dev;}

    void setTimeout(qint64 time) { timeout = time;}
    qint64 getTimeout() const { return timeout;}

private:
    FT_HANDLE handle;
    UCHAR bitmode;
    UCHAR word_length;
    UCHAR stop_bits;
    UCHAR parity;
    qint32 baud_rate;
    qint64 num_dev;
    qint64 timeout;

    class WorkThread : public QThread
    {
    public:
        WorkThread(QFTDI *parent) : QThread(parent) {}
        void run() {
            QFTDI *owner = (QFTDI *)parent();
            while(true) {
                if (owner->bytesAvailable()) {
                    emit owner->readyRead();
                }
                msleep(owner->timeout);
            }
        }
    };
    WorkThread *thread;
};

#endif // QFTDI_H
