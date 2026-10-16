#ifndef BASETASK_H
#define BASETASK_H

#include <string>
#include <cstdint>

class BaseTask
{
protected:
    uint32_t taskIdx;

public:
    BaseTask(uint32_t t_taskIdx, int t_creatorIdx = -1, int t_workerIdx = -1); // Constructor
    virtual ~BaseTask(); // Destructor

    uint32_t getIdx() const; // Get task idx
    int getCreatorIdx() const; // Get creator idx
    int getWorkerIdx() const; // Get worker idx

    int setCreatorIdx(int t_creatorIdx); // Set creator idx
    int setWorkerIdx(int t_workerIdx); // Set worker idx

private:
    int creatorIdx;
    int workerIdx;
};

#endif // BASETASK_H
