#ifndef BASETASK_H
#define BASETASK_H

#include <string>

class BaseTask
{
protected:
    std::string taskName;

public:
    BaseTask(std::string t_taskName, int t_creatorIdx, int t_workerIdx); // Constructor
    virtual ~BaseTask(); // Destructor

    std::string getName(); // Get task name
    int getCreatorIdx(); // Get creator idx
    int getWorkerIdx(); // Get worker idx

private:
    int creatorIdx;
    int workerIdx;
};

#endif // BASETASK_H
