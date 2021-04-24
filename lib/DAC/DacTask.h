#if !defined(__DACTASK_H__)
#define __DACTASK_H__

#include <sample.h>
#include <I2sTask.h>
#include <BufferQueue.h>
#include <driver/i2s.h>
#include <freertos/queue.h>

class DacTask : public I2sTask
{
private:
public:
    DacTask(BufferQueue &sourceQueue)
        : I2sTask(sourceQueue, I2S_NUM_0)
    {
    }

protected:
    virtual void run()
    {
        size_t bufferSize = 64;
        size_t halfSize = bufferSize / 2;
        sample_t buffer[bufferSize];

        setUpDac();

        while (!stopping())
        {
            _bufferQueue.read(buffer, halfSize);

            for (size_t i = halfSize - 1; i > 0; i--)
            {
                buffer[i * 2] = buffer[i];
                buffer[i * 2 + 1] = buffer[i];
            }

            writeToDac(buffer, bufferSize);
            // delay(1000);
            // Serial.println("DacTask::run()");
        }

        tearDownDac();
    }

private:
    void setUpDac()
    {
        i2s_config_t i2sConfig;
        i2sConfig.mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN);
        i2sConfig.sample_rate = 44100;
        i2sConfig.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
        i2sConfig.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
        i2sConfig.intr_alloc_flags = 0; // default interrupt priority
        i2sConfig.dma_buf_count = 8;
        i2sConfig.dma_buf_len = 64;
        i2sConfig.use_apll = false;

        installI2sDriver(i2sConfig);

        i2s_set_pin(_i2s_num, NULL); //for internal DAC, this will enable both of the internal channels

        //You can call i2s_set_dac_mode to set built-in DAC output mode.
        i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN);

        i2s_set_sample_rates(_i2s_num, 22050); //set sample rates
    }

    void tearDownDac()
    {
        uninstallI2sDriver();
    }

    size_t writeToDac(sample_t *pBuffer, size_t size)
    {
        waitEmptyDmaBuffer(I2S_EVENT_TX_DONE);

        size_t written;
        BaseType_t retv = i2s_write(_i2s_num, pBuffer, size, &written, portMAX_DELAY);
        return written;
    }
};

#endif // __DACTASK_H__
