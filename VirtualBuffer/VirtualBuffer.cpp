#include "VirtualBuffer.h"

VirtualBuffer::VirtualBuffer(size_t t_size, uint8_t t_type) {
    // Конструктор
    m_size = t_size;
    m_type = t_type;
}

VirtualBuffer::~VirtualBuffer() {
    // Деструктор
}

int VirtualBuffer::size() {
    // Получение размера буфера
    return(m_size);
}