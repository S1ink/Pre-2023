#pragma once

#include <pigpio.h>


#define BSC_EN  (1 << 0)    // enable bsc
#define BSC_I2  (1 << 2)    // enable i2c mode
#define BSC_BK  (1 << 7)    // abort and clear fifo
#define BSC_TE  (1 << 8)    // transmit enable
#define BSC_RE  (1 << 9)    // receive enable

#define BSC_STARTUP     (BSC_EN | BSC_I2 | BSC_TE | BSC_RE)
#define BSC_SHUTDOWN    (!BSC_EN | !BSC_I2 | BSC_BK)

#define RET_TB  (1 << 0)    // transmit busy
#define RET_RE  (1 << 1)    // receive fifo empty
#define RET_TF  (1 << 2)    // transmit fifo full
#define RET_RF  (1 << 3)    // receive fifo full
#define RET_TE  (1 << 4)    // transmit fifo empty
#define RET_RB  (1 << 5)    // recieve busy

inline uint32_t bsc_control(uint8_t addr, uint16_t flags) { return ((uint32_t)addr << 16) | flags; }
inline int bsc_transmitted(int r) { return (r >> 16) & 0b11111; }
inline int bsc_rfifo(int r) { return (r >> 11) & 0b11111; }
inline int bsc_tfifo(int r) { return (r >> 6) & 0b11111; }
inline bool checkFlag(int s, int f) { return (s & f); }

struct Xfer2 : public bsc_xfer_t {
    void setAddr(uint8_t a, uint16_t f = 0);
    void setControlFlags(uint16_t f);
    int invoke();
    int reset();
};

// void Xfer2::setAddr(uint8_t a, uint16_t f) {
//     this->control = bsc_control(a, f);
// }
// void Xfer2::setControlFlags(uint16_t f) {
//     this->control &= (0xffff0000);
//     this->control |= f;
// }
// int Xfer2::invoke() {
//     return bscXfer(this);
// }