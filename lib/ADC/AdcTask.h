#if !defined(__ADCTASK_H__)
#define __ADCTASK_H__

#include <sample.h>
#include <I2sTask.h>
#include <BufferQueue.h>
#include <driver/i2s.h>
#include <freertos/queue.h>

class AdcTask : public I2sTask
{
private:
    static const adc_unit_t adcUnit = ADC_UNIT_1;
    static const adc1_channel_t adcChannel = ADC1_CHANNEL_7;

public:
    AdcTask(BufferQueue &targetQueue)
        : I2sTask(targetQueue, I2S_NUM_1)
    {
    }

protected:
    void run()
    {
        size_t bufferSize = 64;
        sample_t buffer[bufferSize];

        setUpAdc();

        while (!stopping())
        {
            readFromAdc(buffer, bufferSize);
            _bufferQueue.write(buffer, bufferSize);

            delay(300);
            Serial.println("AdcTask::run()");
        }

        tearDownAdc();
    }

private:
    void setUpAdc()
    {
        i2s_config_t i2sConfig;
        i2sConfig.mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN);
        i2sConfig.sample_rate = 44100;
        i2sConfig.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
        i2sConfig.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
        i2sConfig.communication_format = I2S_COMM_FORMAT_I2S_LSB;
        i2sConfig.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
        i2sConfig.dma_buf_count = 4;
        i2sConfig.dma_buf_len = 64;
        i2sConfig.use_apll = false;
        i2sConfig.tx_desc_auto_clear = false;
        i2sConfig.fixed_mclk = 0;

        installI2sDriver(i2sConfig);

        adc_set_i2s_data_source(ADC_I2S_DATA_SRC_ADC);

        //init ADC pad
        i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_7);
        adc_i2s_mode_init(ADC_UNIT_1, ADC_CHANNEL_7);

        adc1_config_width(ADC_WIDTH_BIT_12);

        // enable the adc
        i2s_adc_enable(_i2s_num);

        i2s_set_sample_rates(_i2s_num, 22050); //set sample rates
    }

    void tearDownAdc()
    {
        uninstallI2sDriver();
    }

    size_t readFromAdc(sample_t *pBuffer, size_t bufferSize)
    {
        waitEmptyDmaBuffer(I2S_EVENT_RX_DONE);

        size_t bytesRead;
        BaseType_t retv = i2s_read(_i2s_num, pBuffer, bufferSize, &bytesRead, portMAX_DELAY);
        Serial.println(bytesRead);
        return bytesRead;
    }
};

#endif // __ADCTASK_H__
