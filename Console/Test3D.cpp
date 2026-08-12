#include "Test3D.h"

#define CGLTF_IMPLEMENTATION
#include "../third_party/cgltf.h"

Test3D::Test3D(bool t_RGB) {
    // Constructor
    std::clog << "Test3D constructor runned..." << std::endl;
    RGB = t_RGB;
}
Test3D::~Test3D() {
    // Destructor
    // buffer.clear();             // Удаляет все строки (размер становится 0)
    // buffer.shrink_to_fit();     // Освобождает выделенную под строки память
}

// Вспомогательная функция для получения указателя на данные из accessor
uint8_t* get_buffer_pointer(const cgltf_accessor* accessor) {
    cgltf_buffer_view* view = accessor->buffer_view;
    return (uint8_t*)view->buffer->data + view->offset + accessor->offset;
}

void Test3D::drawTriangle(const Triangle &triangle, Color color) {
    // Если треугольник абсолютно плоский по вертикали, рисовать нечего
    if (triangle.a.second == triangle.c.second) return;

    int total_height = triangle.c.second - triangle.a.second;

    // Растеризуем треугольник построчно
    for (int i = 0; i < total_height; i++) {
        // Текущая координата Y на экране
        int current_y = triangle.a.second + i;

        // Проверяем, не выходит ли Y за границы вашего буфера
        if (current_y < 0 || current_y >= (int)buffer.size()) continue;

        // Определяем, в какой половине треугольника мы находимся
        bool second_half = i > triangle.b.second - triangle.a.second || triangle.b.second == triangle.a.second;
        int segment_height = second_half ? triangle.c.second - triangle.b.second : triangle.b.second - triangle.a.second;

        if (segment_height == 0) continue;

        // Вычисляем интерполяцию (процент прохода по стороне)
        float alpha = (float)i / total_height;
        float beta  = (float)(i - (second_half ? triangle.b.second - triangle.a.second : 0)) / segment_height;

        // Находим левую и правую границу X для текущей строки Y
        // Т.к. мы не знаем, с какой стороны находится точка B, берем min и max
        int xa = triangle.a.first + (triangle.c.first - triangle.a.first) * alpha;
        int xb = second_half ? triangle.b.first + (triangle.c.first - triangle.b.first) * beta
                             : triangle.a.first + (triangle.b.first - triangle.a.first) * beta;

        if (xa > xb) std::swap(xa, xb);

        // Ограничиваем X под размеры текущей строки буфера
        int start_x = std::max(0, xa);
        int end_x   = std::min((int)buffer[current_y].size() - 1, xb);

        // Заполняем строку символами
        for (int x = start_x; x <= end_x; x++) {
            buffer[current_y][x] = color;
        }
    }
}


std::vector<Triangle3D> Test3D::cube(std::array<float, 3> pos, std::array<float, 3> size, std::array<float, 3> rot) {
    std::vector<Triangle3D> mesh;

    float x = pos[0], y = pos[1], z = pos[2];
    float dx = size[0] * 0.5f, dy = size[1] * 0.5f, dz = size[2] * 0.5f;

    // Определяем 8 вершин куба
    std::array<float, 3> v0 = {x - dx, y - dy, z - dz}; // Лево-Низ-Тыл
    std::array<float, 3> v1 = {x + dx, y - dy, z - dz}; // Право-Низ-Тыл
    std::array<float, 3> v2 = {x + dx, y + dy, z - dz}; // Право-Верх-Тыл
    std::array<float, 3> v3 = {x - dx, y + dy, z - dz}; // Лево-Верх-Тыл
    std::array<float, 3> v4 = {x - dx, y - dy, z + dz}; // Лево-Низ-Фронт
    std::array<float, 3> v5 = {x + dx, y - dy, z + dz}; // Право-Низ-Фронт
    std::array<float, 3> v6 = {x + dx, y + dy, z + dz}; // Право-Верх-Фронт
    std::array<float, 3> v7 = {x - dx, y + dy, z + dz}; // Лево-Верх-Фронт

    // Задняя грань
    mesh.emplace_back(Triangle3D(v0, v1, v2));
    mesh.emplace_back(Triangle3D(v0, v2, v3));

    // Передняя грань
    mesh.emplace_back(Triangle3D(v4, v6, v5));
    mesh.emplace_back(Triangle3D(v4, v7, v6));

    // Левая грань
    mesh.emplace_back(Triangle3D(v4, v1, v0)); // исправлено направление для обхода
    mesh.emplace_back(Triangle3D(v4, v5, v1));

    // Правая грань
    mesh.emplace_back(Triangle3D(v3, v2, v6));
    mesh.emplace_back(Triangle3D(v3, v6, v7));

    // Верхняя грань
    mesh.emplace_back(Triangle3D(v0, v3, v7));
    mesh.emplace_back(Triangle3D(v0, v7, v4));

    // Нижняя грань
    mesh.emplace_back(Triangle3D(v1, v5, v6));
    mesh.emplace_back(Triangle3D(v1, v6, v2));

    for (Triangle3D& t3d : mesh) {
        t3d.rotateXY(pos, rot[1]);
        t3d.rotateYZ(pos, rot[2]);
        t3d.rotateXZ(pos, rot[0]);
    }

    return mesh;
}

std::vector<Triangle3D> Test3D::model(std::array<float, 3> pos, std::array<float, 3> size, std::array<float, 3> rot, const char* file) {
    std::vector<Triangle3D> mesh;

    if (!model_triangles_loaded){
        cgltf_options options = {};
        cgltf_data* data = nullptr;

        // 1. Парсим структуру файла
        if (cgltf_parse_file(&options, file, &data) != cgltf_result_success) {
            std::cerr << "Ошибка при парсинге файла " << file << std::endl;
            return mesh;
        }

        // 2. Загружаем бинарные данные (раскомментировано!)
        if (cgltf_load_buffers(&options, data, file) != cgltf_result_success) {
            std::cerr << "Ошибка при загрузке буферов данных для " << file << std::endl;
            cgltf_free(data);
            return mesh;
        }

        // 3. Собираем геометрию из всех мешей
        for (cgltf_size i = 0; i < data->meshes_count; ++i) {
            cgltf_mesh& gltf_mesh = data->meshes[i];

            for (cgltf_size j = 0; j < gltf_mesh.primitives_count; ++j) {
                cgltf_primitive& primitive = gltf_mesh.primitives[j];

                // Нам нужны только треугольники
                if (primitive.type != cgltf_primitive_type_triangles) continue;

                // Ищем атрибут позиций
                const cgltf_accessor* pos_acc = nullptr;
                for (cgltf_size a = 0; a < primitive.attributes_count; ++a) {
                    if (primitive.attributes[a].type == cgltf_attribute_type_position) {
                        pos_acc = primitive.attributes[a].data;
                        break;
                    }
                }

                if (!pos_acc) continue;

                // Вспомогательная лямбда: читает координаты из GLB, применяет размер(scale) и позицию(translation)
                auto get_vertex = [&](cgltf_size index) -> std::array<float, 3> {
                    float v[3] = {0.0f, 0.0f, 0.0f};
                    cgltf_accessor_read_float(pos_acc, index, v, 3);
                    return {v[0], v[1], v[2]};
                };

                // Если есть массив индексов (оптимизированная сетка)
                if (primitive.indices) {
                    const cgltf_accessor* ind_acc = primitive.indices;
                    for (cgltf_size k = 0; k < ind_acc->count; k += 3) {
                        cgltf_size i0 = cgltf_accessor_read_index(ind_acc, k);
                        cgltf_size i1 = cgltf_accessor_read_index(ind_acc, k + 1);
                        cgltf_size i2 = cgltf_accessor_read_index(ind_acc, k + 2);

                        model_triangles_cach.emplace_back(Triangle3D(get_vertex(i0), get_vertex(i1), get_vertex(i2)));
                    }
                }
                // Если индексов нет, треугольники идут просто по порядку (каждые 3 вершины)
                else {
                    for (cgltf_size k = 0; k < pos_acc->count; k += 3) {
                        model_triangles_cach.emplace_back(Triangle3D(get_vertex(k), get_vertex(k + 1), get_vertex(k + 2)));
                    }
                }
            }
        }

        cgltf_free(data);

        model_triangles_loaded = true;
    }

    // Затем в цикле трансформации кадра:
    for (const Triangle3D& src : model_triangles_cach) {
        Triangle3D t3d = src;
        // 1. Сначала масштаб (Scale)
        t3d.a[0] *= size[0]; t3d.a[1] *= size[1]; t3d.a[2] *= size[2];
        t3d.b[0] *= size[0]; t3d.b[1] *= size[1]; t3d.b[2] *= size[2];
        t3d.c[0] *= size[0]; t3d.c[1] *= size[1]; t3d.c[2] *= size[2];

        // 2. Вращение
        t3d.rotateXY({0,0,0}, rot[1]);
        t3d.rotateYZ({0,0,0}, rot[2]);
        t3d.rotateXZ({0,0,0}, rot[0]);

        // 3. Смещение в мир (Translation)
        t3d.a[0] += pos[0]; t3d.a[1] += pos[1]; t3d.a[2] += pos[2];
        t3d.b[0] += pos[0]; t3d.b[1] += pos[1]; t3d.b[2] += pos[2];
        t3d.c[0] += pos[0]; t3d.c[1] += pos[1]; t3d.c[2] += pos[2];

        mesh.push_back(t3d);
    }

    return mesh;
}

void Test3D::writeTo(std::vector<std::string>* stringsBuffer){
    // Write to buffer
    for (const auto& row : buffer) {
        std::string string_row = "";
        const Color* ocol = nullptr;
        for (const auto& col : row) {
            if (RGB) {
                if (!ocol || col != *ocol) {
                    string_row += std::format("\033[48;2;{};{};{}m", col[0], col[1], col[2]);
                }
                string_row += " ";
            }
            else {
                float b = std::min(std::max((col[0] + col[1] + col[2]) / 255.0f / 3.0f, 0.0f), 1.0f);
                char s = PALETTE[int( (PALETTE.size() - 1.0f) * b )];
                string_row += s;

            }
            ocol = &col;
        }
        string_row += "\033[0m";
        stringsBuffer->emplace_back(std::move(string_row));
    }
}

void Test3D::sortTriangles3D(std::vector<Triangle3D>& triangles) {
    std::sort(triangles.begin(), triangles.end(), [this](const Triangle3D& a, const Triangle3D& b) {
        return a.getZView(camera) > b.getZView(camera); // От большого Z к меньшему
    });
}

void Test3D::calc_buffer(float time, int width, int height) {
    if (width <= 0 || height <= 0) {
        std::cerr << "Error (Test3D::calc_buffer): invalid width or height" << std::endl;
        return;
    }

    if (width != old_width || height != old_height){
        buffer.assign(height, std::vector<Color>(width, {0, 0, 0}));
    }
    else {
        for (auto& row : buffer) {
            std::fill(row.begin(), row.end(), Color{0, 0, 0});
        }
    }

    std::array<float, 3> light = {0,1,0};
    float beta = std::fmod(time * 90.0f, 360.0f);

    std::vector<Triangle3D> mesh = model({0.0f, -0.45f, 1.25f}, {0.05f, 0.05f, 0.05f}, {beta - 90.0f - 22.5f - 11.25f, 0.0f, -90.0f});

    sortTriangles3D(mesh); // Сортируем для более-менее правильной отрисовки, но у нас тут ленивый метод

    for (const Triangle3D& t3d : mesh) {
        Triangle triangle = t3d.get2D(camera, width, height, light);

        if (!triangle.life) continue;
        float b = triangle.blight;
        Color s = t3d.getColor();
        s = {s[0]*b, s[1]*b, s[2]*b};
        drawTriangle(triangle, s);
    }

    old_width = width; old_height = height;
}
