#ifndef BASETASK_H
#define BASETASK_H

#include <string>

class BaseTask
{
protected:
    std::string taskName;

public:
    BaseTask(std::string t_taskName, int t_creatorIdx = -1, int t_workerIdx = -1); // Constructor
    virtual ~BaseTask(); // Destructor

    std::string getName(); // Get task name
    int getCreatorIdx(); // Get creator idx
    int getWorkerIdx(); // Get worker idx

    int setCreatorIdx(int t_creatorIdx); // Set creator idx
    int setWorkerIdx(int t_workerIdx); // Set worker idx

private:
    int creatorIdx;
    int workerIdx;
};

#endif // BASETASK_H
