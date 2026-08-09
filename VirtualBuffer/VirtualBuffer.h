#ifndef VIRTUALBUFFER_H
#define VIRTUALBUFFER_H

#include <cstddef> // For size_t
#include <cstdint> // For uint8_t
#include <vector>  // For std::vector

class VirtualBuffer
{
public:
    VirtualBuffer(size_t t_size, uint8_t t_type);
    ~VirtualBuffer();

    int size(); // Get buffer size
private:
    size_t m_size;
    uint8_t m_type; // 0 - internet mode, 1 - 1
    std::vector<uint8_t> m_buffer;
};

#endif // VIRTUALBUFFER_H
