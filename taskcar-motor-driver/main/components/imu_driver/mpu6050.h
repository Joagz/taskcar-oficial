/**
 * @author Joaquín Gómez (https://www.github.com/Joagz)
 * @details MPU6050 driver for ESPRESSIF SDK ESP32
 */

#ifndef MPU6050_H
#define MPU6050_H

#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdbool.h>

/* Register Map Definition */
#define MPU6050_ADDR_SELF_TEST_X           0x0D
#define MPU6050_ADDR_SELF_TEST_Y           0x0E
#define MPU6050_ADDR_SELF_TEST_Z           0x0F
#define MPU6050_ADDR_SELF_TEST_A           0x10
#define MPU6050_ADDR_SMPLRT_DIV            0x19
#define MPU6050_ADDR_CONFIG                0x1A
#define MPU6050_ADDR_GYRO_CONFIG           0x1B
#define MPU6050_ADDR_ACCEL_CONFIG          0x1C
#define MPU6050_ADDR_FIFO_EN               0x23
#define MPU6050_ADDR_I2C_MST_CTRL          0x24
#define MPU6050_ADDR_I2C_SLV0_ADDR         0x25
#define MPU6050_ADDR_I2C_SLV0_REG          0x26
#define MPU6050_ADDR_I2C_SLV0_CTRL         0x27
#define MPU6050_ADDR_I2C_SLV1_ADDR         0x28
#define MPU6050_ADDR_I2C_SLV1_REG          0x29
#define MPU6050_ADDR_I2C_SLV1_CTRL         0x2A
#define MPU6050_ADDR_I2C_SLV3_ADDR         0x2E
#define MPU6050_ADDR_I2C_SLV3_REG          0x2F
#define MPU6050_ADDR_I2C_SLV3_CTRL         0x30
#define MPU6050_ADDR_I2C_SLV4_ADDR         0x31
#define MPU6050_ADDR_I2C_SLV4_REG          0x32
#define MPU6050_ADDR_I2C_SLV4_DO           0x33
#define MPU6050_ADDR_I2C_SLV4_CTRL         0x34
#define MPU6050_ADDR_I2C_SLV4_DI           0x35
#define MPU6050_ADDR_I2C_MST_STATUS        0x36
#define MPU6050_ADDR_INT_PIN_CFG           0x37
#define MPU6050_ADDR_INT_ENABLE            0x38
#define MPU6050_ADDR_INT_STATUS            0x3A
#define MPU6050_ADDR_ACCEL_XOUT_H          0x3B
#define MPU6050_ADDR_ACCEL_XOUT_L          0x3C
#define MPU6050_ADDR_ACCEL_YOUT_H          0x3D
#define MPU6050_ADDR_ACCEL_YOUT_L          0x3E
#define MPU6050_ADDR_ACCEL_ZOUT_H          0x3F
#define MPU6050_ADDR_ACCEL_ZOUT_L          0x40
#define MPU6050_ADDR_TEMP_OUT_H            0x41
#define MPU6050_ADDR_TEMP_OUT_L            0x42
#define MPU6050_ADDR_GYRO_XOUT_H           0x43
#define MPU6050_ADDR_GYRO_XOUT_L           0x44
#define MPU6050_ADDR_GYRO_YOUT_H           0x45
#define MPU6050_ADDR_GYRO_YOUT_L           0x46
#define MPU6050_ADDR_GYRO_ZOUT_H           0x47
#define MPU6050_ADDR_GYRO_ZOUT_L           0x48
#define MPU6050_ADDR_EXT_SENS_DATA_00      0x49
#define MPU6050_ADDR_EXT_SENS_DATA_01      0x4A
#define MPU6050_ADDR_EXT_SENS_DATA_02      0x4B
#define MPU6050_ADDR_EXT_SENS_DATA_03      0x4C
#define MPU6050_ADDR_EXT_SENS_DATA_04      0x4D
#define MPU6050_ADDR_EXT_SENS_DATA_05      0x4E
#define MPU6050_ADDR_EXT_SENS_DATA_06      0x4F
#define MPU6050_ADDR_EXT_SENS_DATA_07      0x50
#define MPU6050_ADDR_EXT_SENS_DATA_08      0x51
#define MPU6050_ADDR_EXT_SENS_DATA_09      0x52
#define MPU6050_ADDR_EXT_SENS_DATA_10      0x53
#define MPU6050_ADDR_EXT_SENS_DATA_11      0x54
#define MPU6050_ADDR_EXT_SENS_DATA_12      0x55
#define MPU6050_ADDR_EXT_SENS_DATA_13      0x56
#define MPU6050_ADDR_EXT_SENS_DATA_14      0x57
#define MPU6050_ADDR_EXT_SENS_DATA_15      0x58
#define MPU6050_ADDR_EXT_SENS_DATA_16      0x59
#define MPU6050_ADDR_EXT_SENS_DATA_17      0x5A
#define MPU6050_ADDR_EXT_SENS_DATA_18      0x5B
#define MPU6050_ADDR_EXT_SENS_DATA_19      0x5C
#define MPU6050_ADDR_EXT_SENS_DATA_20      0x5D
#define MPU6050_ADDR_EXT_SENS_DATA_21      0x5E
#define MPU6050_ADDR_EXT_SENS_DATA_22      0x5F
#define MPU6050_ADDR_EXT_SENS_DATA_23      0x60
#define MPU6050_ADDR_I2C_SLV0_DO           0x63
#define MPU6050_ADDR_I2C_SLV1_DO           0x64
#define MPU6050_ADDR_I2C_SLV2_DO           0x65
#define MPU6050_ADDR_I2C_SLV3_DO           0x66
#define MPU6050_ADDR_I2C_MST_DELAY_CTRL    0x67
#define MPU6050_ADDR_SIGNAL_PATH_RESET     0x68
#define MPU6050_ADDR_USER_CTRL             0x6A
#define MPU6050_ADDR_PWR_MGMT_1            0x6B
#define MPU6050_ADDR_PWR_MGMT_2            0x6C
#define MPU6050_ADDR_FIFO_COUNTH           0x72
#define MPU6050_ADDR_FIFO_COUNTL           0x73
#define MPU6050_ADDR_FIFO_R_W              0x74
#define MPU6050_ADDR_WHO_AM_I              0x75

#define MPU6050_SLAVE_ADDR_1               0x68
#define MPU6050_SLAVE_ADDR_2               0x69

#define MPU6050_ACCEL_2G                   0x02
#define MPU6050_ACCEL_4G                   0x04
#define MPU6050_ACCEL_8G                   0x08
#define MPU6050_ACCEL_16G                  0x10

#define MPU6050_ACCEL_CONFIG_2G            0b00000000
#define MPU6050_ACCEL_CONFIG_4G            0b00001000
#define MPU6050_ACCEL_CONFIG_8G            0b00010000
#define MPU6050_ACCEL_CONFIG_16G           0b00011000

#define MPU6050_ACCEL_CONFIG_2G_SELF_TEST  0b11100000
#define MPU6050_ACCEL_CONFIG_4G_SELF_TEST  0b11101000
#define MPU6050_ACCEL_CONFIG_8G_SELF_TEST  0b11110000
#define MPU6050_ACCEL_CONFIG_16G_SELF_TEST 0b11111000

#define MPU6050_GYRO_250                   250
#define MPU6050_GYRO_500                   500
#define MPU6050_GYRO_1000                  1000
#define MPU6050_GYRO_2000                  2000

#define MPU6050_GYRO_CONFIG_250            0b00000000
#define MPU6050_GYRO_CONFIG_500            0b00001000
#define MPU6050_GYRO_CONFIG_1000           0b00010000
#define MPU6050_GYRO_CONFIG_2000           0b00011000

#define MPU6050_GYRO_CONFIG_250_SELF_TEST  0b11100000
#define MPU6050_GYRO_CONFIG_500_SELF_TEST  0b11101000
#define MPU6050_GYRO_CONFIG_1000_SELF_TEST 0b11110000
#define MPU6050_GYRO_CONFIG_2000_SELF_TEST 0b11111000

#define MPU6050_SELF_TEST_YES              0b11100000
#define MPU6050_SELF_TEST_NO               0b00000000

#define MPU6050_DLPF_0                     0b00000000
#define MPU6050_DLPF_1                     0b00000001
#define MPU6050_DLPF_2                     0b00000010
#define MPU6050_DLPF_3                     0b00000011
#define MPU6050_DLPF_4                     0b00000100
#define MPU6050_DLPF_5                     0b00000101
#define MPU6050_DLPF_6                     0b00000110

#define MPU6050_TIMEOUT_MS                 16
#define MPU6050_DATA_BUFFER_SIZE           14

/**
 * @brief MPU6050 Structure for configuration purposes
 * @param AFS_SEL 2-bit unsigned value. Selects the full scale range of accelerometers. (Use MPU6050_ACCEL_*G)
 * @param GFS_SEL 2-bit unsigned value. Selects the full scale range of gyroscopes. (Use MPU6050_GYRO_*G)
 * @param DLPF_CFG 3-bit unsigned value. Configures the DLPF setting (Digital Low Pass Filter)
 * @param ADDRESS WHO_AM_I register value, commonly called slave address (by default should be 0x68)
 */
typedef struct
{
    uint8_t AFS_SEL;  /*!< ACCELEROMETER FULL SCALE RANGE */
    uint8_t GFS_SEL;  /*!< GYROSCOPE FULL SCALE RANGE */
    uint8_t DLPF_CFG; /*!< DIGITAL LOW PASS FILTER CONFIG */
    uint8_t ADDRESS;  /*!< MPU6050 SLAVE ADDRESS, BY DEFAULT THIS SHOULD BE 'MPU6050_SLAVE_ADDR_1' (0x68) */

    float x_acc_offset; /*!< (INTERNAL USE) x ACCELERATION OFFSET */
    float y_acc_offset; /*!< (INTERNAL USE) y ACCELERATION OFFSET */
    float z_acc_offset; /*!< (INTERNAL USE) z ACCELERATION OFFSET */

    float x_deg_offset; /*!< (INTERNAL USE) x ROTATION OFFSET */
    float y_deg_offset; /*!< (INTERNAL USE) y ROTATION OFFSET */
    float z_deg_offset; /*!< (INTERNAL USE) z ROTATION OFFSET */

    float x_acc; /*!< X AXIS ACCELEROMETER RAW SCALED DATA */
    float y_acc; /*!< Y AXIS ACCELEROMETER RAW SCALED DATA */
    float z_acc; /*!< Z AXIS ACCELEROMETER RAW SCALED DATA */

    float x_deg; /*!< X AXIS GYROSCOPE RAW SCALED DATA */
    float y_deg; /*!< Y AXIS GYROSCOPE RAW SCALED DATA */
    float z_deg; /*!< Z AXIS GYROSCOPE RAW SCALED DATA */

    float temp; /*!< TEMPERATURE DATA */

    bool x_acc_frw; /*!< X IS FORWARD (POSITIVE OR NEGATIVE ACCELERATION) */
    bool y_acc_frw; /*!< Y IS FORWARD (POSITIVE OR NEGATIVE ACCELERATION) */
    bool z_acc_frw; /*!< Z IS FORWARD (POSITIVE OR NEGATIVE ACCELERATION) */

    bool x_deg_clkw; /*!< X ROTATION IS CLOCKWISE */
    bool y_deg_clkw; /*!< Y ROTATION IS CLOCKWISE */
    bool z_deg_clkw; /*!< Z ROTATION IS CLOCKWISE */
} mpu6050_t;

/**
 * @brief Configure MPU6050's registers and set I2C default configuration
 * @returns ESP_OK if configured correctly, an error if not
 */
esp_err_t mpu6050_configure(mpu6050_t *mpu6050);

/**
 * @brief Get accelerometer and gyroscope data
 * @returns ESP_OK if read correctly, an error if not
 */
esp_err_t mpu6050_accelgyro_data(mpu6050_t *mpu6050);

/**
 * @brief Calibrate MPU6050. The sensor must be static and positioned correctly.
 * This overwrites acceleration values from gyroscope and accelerometer. To obtain raw values add the offsets to
 * the corresponding attributes.
 * @returns ESP_OK if calibrated, an error if not
 */
esp_err_t mpu6050_calibrate(mpu6050_t *mpu6050);

/**
 * @brief Returns a correctly initialized instance of mpu6050_t
 * @param AFS_SEL 2-bit unsigned value. Selects the full scale range of accelerometers. (Use MPU6050_ACCEL_*G)
 * @param GFS_SEL 2-bit unsigned value. Selects the full scale range of gyroscopes. (Use MPU6050_GYRO_*G)
 * @param DLPF_CFG 3-bit unsigned value. Configures the DLPF setting (Digital Low Pass Filter)
 * @param ADDRESS WHO_AM_I register value, commonly called slave address (by default should be 0x68)
 * @returns mpu6050_t empty struct
 */
mpu6050_t mpu6050_new(uint8_t AFS_SEL,
                      uint8_t GFS_SEL,
                      uint8_t DLPF_CFG,
                      uint8_t ADDRESS);

esp_err_t mpu6050_stddev(mpu6050_t *mpu6050, int passes);

#endif