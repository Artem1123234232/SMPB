#include <smpb/BaseTask.h>

BaseTask::BaseTask(uint32_t t_taskIdx)
 : taskIdx(t_taskIdx) {
    // Constructor
}

BaseTask::~BaseTask() = default; // Destructor

uint32_t BaseTask::getIdx() const {
    // Get task idx
    return taskIdx;
}

int BaseTask::getCreatorIdx() const {
    // Get creator idx
    return creatorIdx;
}
int BaseTask::getWorkerIdx() const {
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