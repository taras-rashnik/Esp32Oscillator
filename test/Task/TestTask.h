#if !defined(__TEST_TASK_H__)
#define __TEST_TASK_H__

#include <Arduino.h>
#include <Task.h>

class TestTask : public Task
{
private:
public:
    TestTask() {}

protected:
    virtual void run()
    {
        int i = 0;
        while (!stopping())
        {
            delay(10);
            if (i++ > 100)
                break;
        }
    }
};

#endif // __TEST_TASK_H__
