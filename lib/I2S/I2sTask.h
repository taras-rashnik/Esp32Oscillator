#if !defined(__I2STASK_H__)
#define __I2STASK_H__

#include <Task.h>
#include <BufferQueue.h>
#include <driver/i2s.h>
#include <freertos/queue.h>

class I2sTask : public Task
{
protected:
    BufferQueue &_bufferQueue;
    const i2s_port_t _i2s_num;

private:
    QueueHandle_t _dmaQueue;

public:
    I2sTask(BufferQueue &bufferQueue, i2s_port_t i2s_num)
        : _bufferQueue(bufferQueue), _i2s_num(i2s_num)
    {
    }

protected:
    void installI2sDriver(i2s_config_t &i2sConfig)
    {
        i2s_driver_install(_i2s_num, &i2sConfig, 8, &_dmaQueue); //install and start i2s driver
    }

    void uninstallI2sDriver()
    {
        i2s_driver_uninstall(_i2s_num); //stop & destroy i2s driver
    }

    void waitEmptyDmaBuffer(i2s_event_type_t event_type)
    {
        BaseType_t retv;

        do // wait on I2S event queue until a TX_DONE is found
        {
            i2s_event_t evt;
            retv = xQueueReceive(_dmaQueue, &evt, portMAX_DELAY);
            if ((retv == pdPASS) && (evt.type == event_type)) //I2S DMA finish sent 1 buffer
            {
                break;
            }

            delay(1); // make sure there's time for some other processing if we need to wait!
        } while (retv == pdPASS);
    }
};

#endif // __I2STASK_H__
