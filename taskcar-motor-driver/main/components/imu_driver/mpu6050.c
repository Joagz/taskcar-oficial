#include "mpu6050.h"

static esp_err_t set_i2c(void)
{
    i2c_config_t config = {};
    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = 21;
    config.scl_io_num = 22;
    config.sda_pullup_en = true;
    config.scl_pullup_en = true;
    config.master.clk_speed = 400000;
    config.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, ESP_INTR_FLAG_LEVEL1));

    return ESP_OK;
}

void write_i2c(uint8_t data[2], uint8_t address)
{
    if (data == NULL)
        return;

    if (address == 0x00)
        return;

    ESP_ERROR_CHECK(
        i2c_master_write_to_device(
            I2C_NUM_0,
            address,
            data,
            2,
            pdMS_TO_TICKS(MPU6050_TIMEOUT_MS)));
}

int get_AFS_SEL(uint8_t FS_SEL, uint8_t SELF_TEST_MODE)
{
    if (SELF_TEST_MODE != 0b111 || SELF_TEST_MODE != 0b000)
        SELF_TEST_MODE = 0;

    switch (FS_SEL)
    {
    case MPU6050_ACCEL_2G:
        return 0b00000000 | SELF_TEST_MODE;
        break;
    case MPU6050_ACCEL_4G:
        return 0b00001000 | SELF_TEST_MODE;
        break;
    case MPU6050_ACCEL_8G:
        return 0b00010000 | SELF_TEST_MODE;
        break;
    case MPU6050_ACCEL_16G:
        return 0b00011000 | SELF_TEST_MODE;
        break;
    default:
        return MPU6050_ACCEL_CONFIG_2G;
    }
}

int get_GFS_SEL(uint8_t FS_SEL, uint8_t SELF_TEST_MODE)
{
    if (SELF_TEST_MODE != 0b111 || SELF_TEST_MODE != 0b000)
        SELF_TEST_MODE = 0;

    switch (FS_SEL)
    {
    case MPU6050_GYRO_250:
        return 0b00000000 | SELF_TEST_MODE;
        break;
    case MPU6050_GYRO_500:
        return 0b00001000 | SELF_TEST_MODE;
        break;
    case MPU6050_GYRO_1000:
        return 0b00010000 | SELF_TEST_MODE;
        break;
    case MPU6050_GYRO_2000:
        return 0b00011000 | SELF_TEST_MODE;
        break;
    default:
        return MPU6050_GYRO_250;
    }
}

int get_DLPF(int8_t DLPF)
{
    if (DLPF == 0b00000111 || (DLPF & 0xfffff000) != 0)
    {
        return MPU6050_DLPF_0;
    }

    return DLPF;
}

void mpu6050_configure(uint8_t AFS_SEL, uint8_t GFS_SEL, int8_t DLPF_CFG, bool who_am_i_switch)
{
    ESP_ERROR_CHECK(set_i2c());

    uint8_t data[2] = {0};
    uint8_t device_addr = MPU6050_SLAVE_ADDR_1;
    uint8_t dlpf_cfg = get_DLPF(DLPF_CFG);

    if (who_am_i_switch)
    {
        device_addr = MPU6050_SLAVE_ADDR_2;
    }

    // Wake up the MPU6050 (default is sleep mode)
    data[0] = MPU6050_ADDR_PWR_MGMT_1;
    data[1] = 0x00;

    write_i2c(data, device_addr);

    // Configure DLPF setting for gyroscope and accelerometers
    data[0] = MPU6050_ADDR_CONFIG;
    data[1] = dlpf_cfg;

    write_i2c(data, device_addr);

    // Configure acceleration
    data[0] = MPU6050_ADDR_ACCEL_CONFIG;
    data[1] = get_AFS_SEL(AFS_SEL, 0);

    write_i2c(data, device_addr);

    // Configure gyroscope
    data[0] = MPU6050_ADDR_GYRO_CONFIG;
    data[1] = get_GFS_SEL(GFS_SEL, 0);

    write_i2c(data, device_addr);
}

esp_err_t mpu6050_accelgyro_data(float *a, float *g, uint8_t AFS_SEL, uint8_t GFS_SEL)
{
    if (a == NULL || g == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t rx_data[MPU6050_DATA_BUFFER_SIZE];
    uint8_t command = MPU6050_ADDR_ACCEL_XOUT_H;

    esp_err_t err = i2c_master_write_read_device(
        I2C_NUM_0, MPU6050_SLAVE_ADDR_1, &command, sizeof(command),
        rx_data, MPU6050_DATA_BUFFER_SIZE, pdMS_TO_TICKS(MPU6050_TIMEOUT_MS));

    if (err == ESP_OK)
    {
        int16_t xAcc = (rx_data[0] << 8) | rx_data[1];
        int16_t yAcc = (rx_data[2] << 8) | rx_data[3];
        int16_t zAcc = (rx_data[4] << 8) | rx_data[5];

        int16_t xGyro = (rx_data[8] << 8) | rx_data[9];
        int16_t yGyro = (rx_data[10] << 8) | rx_data[11];
        int16_t zGyro = (rx_data[12] << 8) | rx_data[13];

        float accScaleFactor = (float)(AFS_SEL) / 32768.0;
        float gyroScaleFactor = (float)(GFS_SEL) / 32768.0;

        a[0] = xAcc * accScaleFactor * 9.8;
        a[1] = yAcc * accScaleFactor * 9.8;
        a[2] = zAcc * accScaleFactor * 9.8;

        g[0] = xGyro * gyroScaleFactor;
        g[1] = yGyro * gyroScaleFactor;
        g[2] = zGyro * gyroScaleFactor;

        return ESP_OK;
    }

    return ESP_FAIL;
}