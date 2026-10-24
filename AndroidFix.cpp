#ifdef __ANDROID__

#include <cstdlib>

// Эта функция жестко требуется линкером Android, если бинарник собран через NDK r27.
// Мы объявляем её здесь, чтобы она вшилась внутрь исполняемого файла как заглушка.
extern "C" [[noreturn]] void _ZNSt6__ndk122__libcpp_verbose_abortEPKcz(const char* format, ...) {
    std::abort();
}

#endif
