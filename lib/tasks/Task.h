#if !defined(__TASK_H__)
#define __TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Task
{
private:
    const char *_name = "No name";
    uint32_t _stackSize = 2048;
    UBaseType_t _priority = 1;
    BaseType_t _core = 1;
    TaskHandle_t _taskHandle = 0;

    volatile bool _running = false;
    volatile bool _stopping = false;

public:
    void start()
    {
        BaseType_t res = xTaskCreatePinnedToCore(
            Task::taskProc,
            _name,
            _stackSize,
            this,
            _priority,
            &_taskHandle,
            _core);

        assert(res == pdPASS);
    }

    void stop()
    {
        _stopping = true;

        for (size_t i = 0; i < 20; i++)
        {
            if (!running())
                return;

            delay(50);
        }

        if (_taskHandle != 0)
        {
            vTaskDelete(_taskHandle);
        }
    }

    void join()
    {
        while (running())
        {
            delay(100);
        }
    }

    bool running() { return _running; }

protected:
    virtual void run() = 0;

    bool stopping() { return _stopping; }

private:
    static void taskProc(void *pParams)
    {
        Task *pTask = (Task *)pParams;
        pTask->_running = true;

        pTask->run();

        pTask->_taskHandle = 0;
        pTask->_running = false;
        vTaskDelete(NULL);
    }
};

#endif // __TASK_H__
