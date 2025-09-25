#include <stdint.h>

#define MPU_TYPE (*(volatile uint32_t*)0xE000ED90)
#define MPU_CTRL (*(volatile uint32_t*)0xE000ED94)
#define MPU_RNR  (*(volatile uint32_t*)0xE000ED98)
#define MPU_RBAR (*(volatile uint32_t*)0xE000ED9C)
#define MPU_RASR (*(volatile uint32_t*)0xE000EDA0)

void mpu_init(void) {
    // Disable MPU
    MPU_CTRL = 0;
    
    // Configure region 0 for privileged access only
    MPU_RNR = 0;
    MPU_RBAR = 0x00000000; // Start address
    MPU_RASR = (0x1F << 1) | // Size 4GB
               (0x3 << 3) |  // AP: privileged access only
               (1 << 0);     // Enable region
    
    // Enable MPU
    MPU_CTRL = 1;
}
