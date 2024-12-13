#include "mpu6050.h"
#include <math.h>

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

esp_err_t write_i2c(uint8_t data[2], uint8_t address)
{
    if (data == NULL)
        return ESP_FAIL;

    if (address == 0x00)
        return ESP_FAIL;

    return i2c_master_write_to_device(
        I2C_NUM_0,
        address,
        data,
        2,
        pdMS_TO_TICKS(MPU6050_TIMEOUT_MS));
}

int get_AFS_SEL(int FS_SEL, int SELF_TEST_MODE)
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

int get_GFS_SEL(int FS_SEL, int SELF_TEST_MODE)
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

mpu6050_t mpu6050_new(uint8_t AFS_SEL,
                      uint8_t GFS_SEL,
                      uint8_t DLPF_CFG,
                      uint8_t ADDRESS)
{
    return (mpu6050_t){
        .ADDRESS = ADDRESS,
        .AFS_SEL = AFS_SEL,
        .GFS_SEL = GFS_SEL,
        .DLPF_CFG = DLPF_CFG,

        .x_acc = 0.0f,
        .y_acc = 0.0f,
        .z_acc = 0.0f,

        .x_deg = 0.0f,
        .y_deg = 0.0f,
        .z_deg = 0.0f,

        .x_acc_offset = 0.0f,
        .y_acc_offset = 0.0f,
        .z_acc_offset = 0.0f,
        .x_deg_offset = 0.0f,
        .y_deg_offset = 0.0f,
        .z_deg_offset = 0.0f,

        .temp = 0};
}

esp_err_t mpu6050_configure(mpu6050_t *mpu6050)
{
    if (mpu6050 == NULL)
        return ESP_ERR_INVALID_ARG;

    ESP_ERROR_CHECK(set_i2c());

    uint8_t data[2] = {0};

    esp_err_t err = ESP_OK;

    // Wake up the MPU6050 (default is sleep mode)
    data[0] = MPU6050_ADDR_PWR_MGMT_1;
    data[1] = 0x00;

    if ((err = write_i2c(data, mpu6050->ADDRESS)) != ESP_OK)
    {
        return err;
    }

    // Configure DLPF setting for gyroscope and accelerometers
    data[0] = MPU6050_ADDR_CONFIG;
    data[1] = mpu6050->DLPF_CFG;

    if ((err = write_i2c(data, mpu6050->ADDRESS)) != ESP_OK)
    {
        return err;
    }

    // Configure acceleration
    data[0] = MPU6050_ADDR_ACCEL_CONFIG;
    data[1] = get_AFS_SEL(mpu6050->AFS_SEL, 0);

    if ((err = write_i2c(data, mpu6050->ADDRESS)) != ESP_OK)
    {
        return err;
    }

    // Configure gyroscope
    data[0] = MPU6050_ADDR_GYRO_CONFIG;
    data[1] = get_GFS_SEL(mpu6050->GFS_SEL, 0);

    if ((err = write_i2c(data, mpu6050->ADDRESS)) != ESP_OK)
    {
        return err;
    }

    return ESP_OK;
}

esp_err_t mpu6050_accelgyro_data(mpu6050_t *mpu6050)
{
    if (mpu6050 == NULL)
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

        float accScaleFactor = (float)(mpu6050->AFS_SEL) / 32768.0;
        float gyroScaleFactor = (float)(mpu6050->GFS_SEL) / 32768.0;

        mpu6050->x_acc = (float)(abs(xAcc) * accScaleFactor);
        mpu6050->y_acc = (float)(abs(yAcc) * accScaleFactor);
        mpu6050->z_acc = (float)(abs(zAcc) * accScaleFactor);

        mpu6050->x_deg = (float)(abs(xGyro) * gyroScaleFactor);
        mpu6050->y_deg = (float)(abs(yGyro) * gyroScaleFactor);
        mpu6050->z_deg = (float)(abs(zGyro) * gyroScaleFactor);

        mpu6050->x_acc_frw = xAcc > 0;
        mpu6050->y_acc_frw = yAcc > 0;
        mpu6050->z_acc_frw = zAcc > 0;

        mpu6050->x_deg_clkw = xGyro > 0;
        mpu6050->y_deg_clkw = yGyro > 0;
        mpu6050->z_deg_clkw = zGyro > 0;

        return ESP_OK;
    }

    return ESP_FAIL;
}

esp_err_t mpu6050_calibrate(mpu6050_t *mpu6050)
{
    float xaavg = 0, yaavg = 0, zaavg = 0;
    float xgavg = 0, ygavg = 0, zgavg = 0;

    int passes = 100;

    for (int i = 0; i < passes; i++)
    {
        if (mpu6050_accelgyro_data(mpu6050) != ESP_OK)
            continue;

        xaavg += mpu6050->x_acc;
        xgavg += mpu6050->x_deg;
        yaavg += mpu6050->y_acc;
        ygavg += mpu6050->y_deg;
        zaavg += mpu6050->z_acc;
        zgavg += mpu6050->z_deg;
    }

    xaavg = xaavg / passes;
    yaavg = yaavg / passes;
    zaavg = zaavg / passes;
    xgavg = xgavg / passes;
    ygavg = ygavg / passes;
    zgavg = zgavg / passes;

    mpu6050->x_acc_offset = xaavg;
    mpu6050->y_acc_offset = yaavg;
    mpu6050->z_acc_offset = zaavg;
    mpu6050->x_deg_offset = xgavg;
    mpu6050->y_deg_offset = ygavg;
    mpu6050->z_deg_offset = zgavg;

    return ESP_OK;
}

esp_err_t mpu6050_stddev(mpu6050_t *mpu6050, int passes)
{
    if (mpu6050 == NULL)
    {
        ESP_LOGE("stats", "Invalid MPU6050 pointer");
        return ESP_ERR_INVALID_ARG;
    }

    float xaavg = 0, yaavg = 0, zaavg = 0;
    float xgavg = 0, ygavg = 0, zgavg = 0;

    float xastddev = 0, yastddev = 0, zastddev = 0;
    float xgstddev = 0, ygstddev = 0, zgstddev = 0;

    int successful_passes = 0;

    // First loop: calculate averages
    for (int i = 0; i < passes; i++)
    {
        if (mpu6050_accelgyro_data(mpu6050) != ESP_OK)
            continue;

        xaavg += mpu6050->x_acc;
        xgavg += mpu6050->x_deg;
        yaavg += mpu6050->y_acc;
        ygavg += mpu6050->y_deg;
        zaavg += mpu6050->z_acc;
        zgavg += mpu6050->z_deg;
        successful_passes++;
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    if (successful_passes == 0)
    {
        ESP_LOGE("stats", "No successful readings from MPU6050");
        return ESP_FAIL;
    }

    xaavg /= (float)(successful_passes);
    xgavg /= (float)(successful_passes);
    yaavg /= (float)(successful_passes);
    ygavg /= (float)(successful_passes);
    zaavg /= (float)(successful_passes);
    zgavg /= (float)(successful_passes);

    // Second loop: calculate standard deviation
    for (int i = 0; i < passes; i++)
    {
        if (mpu6050_accelgyro_data(mpu6050) != ESP_OK)
            continue;

        xastddev += powf(mpu6050->x_acc - xaavg, 2);
        xgstddev += powf(mpu6050->x_deg - xgavg, 2);
        yastddev += powf(mpu6050->y_acc - yaavg, 2);
        ygstddev += powf(mpu6050->y_deg - ygavg, 2);
        zastddev += powf(mpu6050->z_acc - zaavg, 2);
        zgstddev += powf(mpu6050->z_deg - zgavg, 2);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    xastddev = sqrtf(xastddev / (float)(successful_passes));
    xgstddev = sqrtf(xgstddev / (float)(successful_passes));
    yastddev = sqrtf(yastddev / (float)(successful_passes));
    ygstddev = sqrtf(ygstddev / (float)(successful_passes));
    zastddev = sqrtf(zastddev / (float)(successful_passes));
    zgstddev = sqrtf(zgstddev / (float)(successful_passes));

    mpu6050->x_acc_offset = xastddev;
    mpu6050->y_acc_offset = xgstddev;
    mpu6050->z_acc_offset = yastddev;
    mpu6050->x_deg_offset = ygstddev;
    mpu6050->y_deg_offset = zastddev;
    mpu6050->z_deg_offset = zgstddev;

    ESP_LOGI("standard deviation",
             "\nxastddev: %.4f\nyastddev: %.4f\nzastddev: %.4f\nxgstddev: %.4f\nygstddev: %.4f\nzgstddev: %.4f",
             xastddev, yastddev, zastddev, xgstddev, ygstddev, zgstddev);

    ESP_LOGI("median",
             "\nxaavg: %.4f\nyaavg: %.4f\nzaavg: %.4f\nxgavg: %.4f\nygavg: %.4f\nzgavg: %.4f",
             xaavg, yaavg, zaavg, xgavg, ygavg, zgavg);

    return ESP_OK;
}
