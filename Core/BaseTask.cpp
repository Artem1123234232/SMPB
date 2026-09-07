#include "BaseTask.h"

// Core class
#include "Core.h"

BaseTask::BaseTask(std::string t_taskName, int t_creatorIdx, int t_workerIdx)
 : taskName(t_taskName), creatorIdx(t_creatorIdx), workerIdx(t_workerIdx) {
    // Constructor
}

BaseTask::~BaseTask() = default; // Destructor

std::string BaseTask::getName() {
    // Get task name
    return taskName;
}

int BaseTask::getCreatorIdx() {
    // Get creator idx
    return creatorIdx;
}
int BaseTask::getWorkerIdx() {
    // Get worker idx
    return workerIdx;
}

int BaseTask::setCreatorIdx(int t_creatorIdx) {
    // Set creator idx
    creatorIdx = t_creatorIdx;
    return 0;
}
int BaseTask::setWorkerIdx(int t_workerIdx) {
    // Set worker idx
    workerIdx = t_workerIdx;
    return 0;
}