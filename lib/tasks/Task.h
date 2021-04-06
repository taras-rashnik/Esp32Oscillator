#if !defined(__TASK_H__)
#define __TASK_H__

class Task
{
private:
    
public:
    void start() {}
    void stop() {}

protected:
    virtual void run() = 0;
};

#endif // __TASK_H__
