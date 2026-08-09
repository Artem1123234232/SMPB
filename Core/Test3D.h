#ifndef TEST3D_H
#define TEST3D_H

// Для работы со строками, списками, вывода в консоль и другие базовые операции
#include <iostream>
#include <format>
#include <utility>
#include <array>
#include <vector>
#include <algorithm>

// Для математики
#include <cmath>
#include <cfloat> // Для FLT_MAX

#define RGB // Включение RGB режима

// Константа для перевода градусов в радианы
const float DEG_TO_RAD = 3.1415926535f / 180.0f;

// Консольная палитра
#ifndef RGB
//const std::string PALETTE = " .:!/r(l1Z4H9W8$@";
//const std::string PALETTE = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
const std::string PALETTE = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$#FX%WM&8@KOB0S523456789";
#endif

const float DIS_TO_BLIGHT_COFF = 1.0f / 5.0f;

// Цвета это три значения от 0 до 255
using Color = std::array<uint8_t, 3>;

struct Point2D {
    int x, y;
    float z_view;
};

struct Camera{
    std::array<float, 3> pos;
    std::pair<float, float> rot;
    float fov;
    float f;
    Camera(float t_fov, std::array<float, 3> t_pos = {0.0f, 0.0f, 0.0f}, std::pair<float, float> t_rot = {0.0f, 0.0f}){
        pos = t_pos; rot = t_rot; fov = t_fov;
        f = 1.0f / std::tan((fov * DEG_TO_RAD) / 2.0f);
    }
    float getZView(std::array<float, 3> dot){
        // 1. Смещение относительно камеры
        float x_local = dot[0] - pos[0];
        float y_local = dot[1] - pos[1];
        float z_local = dot[2] - pos[2];

        // Переводим углы в радианы
        float pitch = rot.first * DEG_TO_RAD;
        float yaw = rot.second * DEG_TO_RAD;

        float cos_p = std::cos(pitch), sin_p = std::sin(pitch);
        float cos_y = std::cos(yaw),   sin_y = std::sin(yaw);

        // 2. Стандартный инвертированный поворот камеры (Камера смотрит вдоль +Z при yaw=0, pitch=0)
        // Поворот вокруг оси Y (Yaw)
        float z_rot1 = sin_y * x_local + cos_y * z_local;

        // Поворот вокруг оси X (Pitch)
        float z_view = -sin_p * y_local + cos_p * z_rot1;

        return z_view;
    }
    Point2D projectGet(std::array<float, 3> dot, int width, int height) {
        // 1. Смещение относительно камеры
        float x_local = dot[0] - pos[0];
        float y_local = dot[1] - pos[1];
        float z_local = dot[2] - pos[2];

        // Переводим углы в радианы
        float pitch = rot.first * DEG_TO_RAD;
        float yaw = rot.second * DEG_TO_RAD;

        float cos_p = std::cos(pitch), sin_p = std::sin(pitch);
        float cos_y = std::cos(yaw),   sin_y = std::sin(yaw);

        // 2. Стандартный инвертированный поворот камеры (Камера смотрит вдоль +Z при yaw=0, pitch=0)
        // Поворот вокруг оси Y (Yaw)
        float x_rot = cos_y * x_local - sin_y * z_local;
        float z_rot1 = sin_y * x_local + cos_y * z_local;

        // Поворот вокруг оси X (Pitch)
        float y_view = cos_p * y_local + sin_p * z_rot1;
        float z_view = -sin_p * y_local + cos_p * z_rot1;
        float x_view = x_rot;

        // Проверка отсечения (теперь всё, что перед камерой по оси Z, будет иметь z_view > 0)
        if (z_view <= 0.001f) {
            return {-1, -1};
        }

        // 3. Перспективная проекция
        float aspect = static_cast<float>(width) / static_cast<float>(height);
        float x_ndc = (x_view * f) / (z_view * aspect);
        float y_ndc = (y_view * f) / z_view;

        // 4. Экранные координаты
        int x_screen = static_cast<int>((x_ndc + 1.0f) / 2.0f * width);
        int y_screen = static_cast<int>((1.0f - y_ndc) / 2.0f * height);

        return Point2D{x_screen, y_screen, z_view};
    }
};

struct Triangle{
    std::pair<int, int> a;
    std::pair<int, int> b;
    std::pair<int, int> c;
    float z;
    float blight;
    bool life = false;
    Triangle() = default;
    Triangle(std::pair<int, int> t_a, std::pair<int, int> t_b, std::pair<int, int> t_c, float t_z = 0.0f, float t_blight = 0.5f, bool t_life = true){
        // 1. Задаем вершины и другое
        a = t_a; b = t_b; c = t_c; z = t_z; blight = t_blight; life = t_life;

        // 2. Сортируем вершины по координате Y (a - верхняя, b - средняя, c - нижняя)
        if (a.second > b.second) std::swap(a, b);
        if (a.second > c.second) std::swap(a, c);
        if (b.second > c.second) std::swap(b, c);
    }
};

struct Triangle3D {
    std::array<float, 3> a;
    std::array<float, 3> b;
    std::array<float, 3> c;

    Triangle3D(std::array<float, 3> t_a, std::array<float, 3> t_b, std::array<float, 3> t_c) {
        a = t_a; b = t_b; c = t_c;
        sortVertices(); // Автоматически упорядочиваем вершины при создании
    }

    float getMaxZView(Camera &camera) const{
        return std::max(camera.getZView(a), std::max(camera.getZView(b), camera.getZView(c)));
    }

    Triangle get2D(Camera &camera, int width, int height, std::array<float, 3> &light) const{
        auto a2d = camera.projectGet(a, width, height);
        auto b2d = camera.projectGet(b, width, height);
        auto c2d = camera.projectGet(c, width, height);

        // Если хоть одна точка улетела за экран (отсечение сзади), не рисуем
        if (a2d.x == -1 || b2d.x == -1 || c2d.x == -1) return Triangle();

        // triangles.emplace_back( Triangle({a2d.x, a2d.y}, {b2d.x, b2d.y}, {c2d.x, c2d.y}, (a2d.z_view + b2d.z_view + c2d.z_view) / 3.0f) );

        // 1. Расчет нормали
        std::array<float, 3> normal = getNormal();

        // 2. Вектор направления от вершины треугольника к камере
        std::array<float, 3> viewDir = {
            camera.pos[0] - a[0],
            camera.pos[1] - a[1],
            camera.pos[2] - a[2]
        };

        // 3. Скалярное произведение (Dot Product) нормали и вектора взгляда
        float cullingDot = normal[0]*viewDir[0] + normal[1]*viewDir[1] + normal[2]*viewDir[2];

        // Если нормаль смотрит ОТ камеры (cullingDot <= 0), полигон не виден — отбрасываем!
        if (cullingDot <= 0.0f) {
            return Triangle(); // life = false
        }

        // 4. Расчет освещения
        float dot = normal[0]*light[0] + normal[1]*light[1] + normal[2]*light[2];

        float blight = std::min(std::max(dot * 0.5f + 0.5f, 0.0f), 1.0f);

        return Triangle({a2d.x, a2d.y}, {b2d.x, b2d.y}, {c2d.x, c2d.y}, std::max(std::max(a2d.z_view, b2d.z_view), c2d.z_view), blight);

    }

    std::array<float, 3> getNormal() const {
        // 1. Вычисляем два вектора ребер: (b - a) и (c - a)
        float edge1_x = b[0] - a[0];
        float edge1_y = b[1] - a[1];
        float edge1_z = b[2] - a[2];

        float edge2_x = c[0] - a[0];
        float edge2_y = c[1] - a[1];
        float edge2_z = c[2] - a[2];

        // 2. Считаем векторное произведение (Cross Product)
        float nx = edge1_y * edge2_z - edge1_z * edge2_y;
        float ny = edge1_z * edge2_x - edge1_x * edge2_z;
        float nz = edge1_x * edge2_y - edge1_y * edge2_x;

        // 3. Находим длину полученного вектора
        float length = std::sqrt(nx * nx + ny * ny + nz * nz);

        // 4. Нормализуем вектор (проверяя на деление на ноль для вырожденных треугольников)
        if (length > 1e-6f) {
            return { nx / length, ny / length, nz / length };
        }

        return { 0.0f, 0.0f, 0.0f }; // Дефолтный вектор для некорректного треугольника
    }

    // 1. Вращение в плоскости XZ (вокруг оси Y)
    void rotateXZ(const std::array<float, 3>& center, float angleDegrees) {
        float rad = angleDegrees * (M_PI / 180.0f);
        float cosA = std::cos(rad);
        float sinA = std::sin(rad);

        rotatePointXZ(a, center, cosA, sinA);
        rotatePointXZ(b, center, cosA, sinA);
        rotatePointXZ(c, center, cosA, sinA);
    }

    // 2. Вращение в плоскости XY (вокруг оси Z)
    void rotateXY(const std::array<float, 3>& center, float angleDegrees) {
        float rad = angleDegrees * (M_PI / 180.0f);
        float cosA = std::cos(rad);
        float sinA = std::sin(rad);

        rotatePointXY(a, center, cosA, sinA);
        rotatePointXY(b, center, cosA, sinA);
        rotatePointXY(c, center, cosA, sinA);
    }

    // 3. Вращение в плоскости YZ (вокруг оси X)
    void rotateYZ(const std::array<float, 3>& center, float angleDegrees) {
        float rad = angleDegrees * (M_PI / 180.0f);
        float cosA = std::cos(rad);
        float sinA = std::sin(rad);

        rotatePointYZ(a, center, cosA, sinA);
        rotatePointYZ(b, center, cosA, sinA);
        rotatePointYZ(c, center, cosA, sinA);
    }

private:
    // Сортировка вершин против часовой стрелки относительно их центра
    void sortVertices() {
        // 1. Находим геометрический центр (барицентр) треугольника
        std::array<float, 3> center = {
            (a[0] + b[0] + c[0]) / 3.0f,
            (a[1] + b[1] + c[1]) / 3.0f,
            ((a[2] + b[2] + c[2]) / 3.0f)
        };

        // 2. Считаем черновую (неотсортированную) нормаль для определения плоскости
        float e1x = b[0] - a[0]; float e1y = b[1] - a[1]; float e1z = b[2] - a[2];
        float e2x = c[0] - a[0]; float e2y = c[1] - a[1]; float e2z = c[2] - a[2];
        std::array<float, 3> norm = {
            e1y * e2z - e1z * e2y,
            e1z * e2x - e1x * e2z,
            e1x * e2y - e1y * e2x
        };

        // Если треугольник вырожден в линию, сортировка невозможна
        if (std::sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]) < 1e-6f) return;

        // 3. Лямбда-функция для вычисления угла между вектором на вершину и вектором 'a'
        auto getAngle = [&](const std::array<float, 3>& p) {
            float v1x = a[0] - center[0]; float v1y = a[1] - center[1]; float v1z = a[2] - center[2];
            float v2x = p[0] - center[0]; float v2y = p[1] - center[1]; float v2z = p[2] - center[2];

            // Скалярное произведение
            float dot = v1x * v2x + v1y * v2y + v1z * v2z;
            // Векторное произведение
            float cx = v1y * v2z - v1z * v2y;
            float cy = v1z * v2x - v1x * v2z;
            float cz = v1x * v2y - v1y * v2x;

            // Направление угла относительно плоскости треугольника
            float sign = (cx * norm[0] + cy * norm[1] + cz * norm[2] > 0) ? 1.0f : -1.0f;

            return sign * dot; // Возвращает псевдо-угол для сортировки
        };

        // 4. Сортируем вершины b и c относительно базовой вершины a
        if (getAngle(b) > getAngle(c)) {
            std::swap(b, c);
        }
    }
    // Вспомогательные приватные методы для изменения одной точки
    void rotatePointXZ(std::array<float, 3>& p, const std::array<float, 3>& center, float cosA, float sinA) {
        float dx = p[0] - center[0];
        float dz = p[2] - center[2];
        p[0] = dx * cosA - dz * sinA + center[0];
        p[2] = dx * sinA + dz * cosA + center[2];
    }

    void rotatePointXY(std::array<float, 3>& p, const std::array<float, 3>& center, float cosA, float sinA) {
        float dx = p[0] - center[0];
        float dy = p[1] - center[1];
        p[0] = dx * cosA - dy * sinA + center[0];
        p[1] = dx * sinA + dy * cosA + center[1];
    }

    void rotatePointYZ(std::array<float, 3>& p, const std::array<float, 3>& center, float cosA, float sinA) {
        float dy = p[1] - center[1];
        float dz = p[2] - center[2];
        p[1] = dy * cosA - dz * sinA + center[1];
        p[2] = dy * sinA + dz * cosA + center[2];
    }
};

class Test3D
{
private:
    int old_width = -1; int old_height = -1;
#ifdef RGB
    std::vector<std::vector<Color>> buffer; // Буфер консоли
#else
    std::vector<std::vector<char>> buffer; // Буфер консоли
#endif
    //std::vector<std::vector<float>> depth_buffer; // Буфер глубины

    // Модель
    bool model_triangles_loaded = false; // Пока не загружена...
    std::vector<Triangle3D> model_triangles_cach; // И кэша нету )=

    Camera camera = Camera(60.0f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}); // Камера
public:
    Test3D(); // Constructor
    ~Test3D(); // Destructor
    void print_data3d(); // Печать данных 3д модели
#ifdef RGB
    void drawTriangle(const Triangle &triangle, Color = {255, 255, 255}); // Отрисовка треугольника
#else
    void drawTriangle(const Triangle &triangle, char color = '@'); // Отрисовка треугольника
#endif
    //void drawLine(int x0, int y0, int x1, int y1, char color); // Отрисовка линии
    //void drawTriangleEdges(const Triangle &triangle, char color); // Отрисовка граней треугольника
    std::vector<Triangle3D> cube(std::array<float, 3> pos, std::array<float, 3> size, std::array<float, 3> rot = {0,0,0});
    std::vector<Triangle3D> model(std::array<float, 3> pos, std::array<float, 3> size, std::array<float, 3> rot = {0,0,0}, const char* file = "model.glb");
    void flush(); // Flush Buffer
    void sortTriangles3D(std::vector<Triangle3D>& triangles); // Функция для сортировки треугольников
    void print(float time, int width, int height); // Печать
    void tick(long long tickCounter, int ticksPerSecond); // Тик
};

#endif // TEST3D_H
