#include "i2c.h"

#include <string.h>


void Xfer2::setAddr(uint8_t a, uint16_t f) {
    this->control = bsc_control(a, f);
}
void Xfer2::setControlFlags(uint16_t f) {
    this->control &= (0xffff0000);
    this->control |= f;
}
int Xfer2::invoke() {
    return bscXfer(this);
}
int Xfer2::reset() {
    this->txCnt = 0;
    return bscXfer(this);
}

void dumpXferState(std::ostream& o, int s) {
    o << bsc_transmitted(s) << " bytes transmitted, "
        << bsc_rfifo(s) << " bytes in receive FIFO, "
        << bsc_tfifo(s) << " bytes in transmit FIFO.\n"
        "Receive empty?: " << checkFlag(s, RET_RE) << ", full?: " << checkFlag(s, RET_RF) << ", busy?: " << checkFlag(s, RET_RB)
        << "\nTransmit empty?: " << checkFlag(s, RET_TE) << ", full?: " << checkFlag(s, RET_TF) << ", busy?: " << checkFlag(s, RET_TB)
        << std::endl;
}


void demo() {
    const int id = 0x08;
    const char* t1 = "AhvIkSwZzlopDMB";
    const char* t2 = "yklsj,vmz.oieuK";

    gpioInitialise();
    int h = i2cOpen(1, id, 0);
    Xfer2 xfer;
    xfer.setAddr(id, BSC_STARTUP);
    std::cout << "Initial state:\n";
    dumpXferState(std::cout, xfer.reset()); // init
    memset(xfer.rxBuf, 0, 16);
    
    xfer.txCnt = 16;    // first write
    memcpy(xfer.txBuf, t1, 16);
    memcpy(xfer.txBuf + 16, t2, 16);
    std::cout << "\nWriting 32 bytes to BSC...\n";
    hexifyBuffer(std::cout, xfer.txBuf, 32);
    dumpXferState(std::cout, xfer.invoke());
    std::cout << "Received " << xfer.rxCnt << " bytes.\n";
    hexifyBuffer(std::cout, xfer.rxBuf, 16);

    char buff[32];  // first read
    // std::cout << "\nMaster read " << i2cReadI2CBlockData(h, id, buff, 16) << " bytes.\n";
    int r = i2cReadDevice(h, buff, 32);
    std::cout << "\n\nMaster read " << r << " bytes.\n";
    hexifyBuffer(std::cout, buff, 32);

    //memcpy(xfer.txBuf, t2, 16); // second write
    xfer.txCnt = 0;
    //std::cout << "\n\nWriting [second] 16 bytes to BSC...\n";
    //hexifyBuffer(std::cout, xfer.txBuf, 16);
    dumpXferState(std::cout, xfer.invoke());
    std::cout << "Received " << xfer.rxCnt << " bytes.\n";
    hexifyBuffer(std::cout, xfer.rxBuf, 16);

    // std::cout << "\nMaster read " << i2cReadI2CBlockData(h, id, buff, 16) << " bytes.\n"; // second read
    std::cout << "\nMaster read " << i2cReadDevice(h, buff, 16) << " bytes.\n";
    hexifyBuffer(std::cout, buff, 16);
    std::cout << "\n\nMaster read " << i2cReadDevice(h, buff, 32 - r) << " bytes.\n";
    hexifyBuffer(std::cout, buff, 32);

    xfer.invoke();
    std::cout << "Received " << xfer.rxCnt << " bytes.\n";
    hexifyBuffer(std::cout, xfer.rxBuf, 16);

    i2cClose(h);
    xfer.setAddr(0, BSC_SHUTDOWN);
    gpioTerminate();
}