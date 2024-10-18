#include <stdio.h>
#include "allocator.h"

int main() {
    // Инициализация аллокатора
    init_allocator();
    printf("Allocator initialized.\n");

    // 1. Нормальный случай выделения памяти
    printf("Test 1: Allocating 100 bytes\n");
    void* p1 = my_malloc(100); // Выделяем 100 байт
    if (p1 != NULL) {
        printf("Success: Memory allocated at address: %p\n", p1); // Успешное выделение, адрес не NULL
    } else {
        printf("Fail: Failed to allocate memory.\n"); // Ожидаем, что выделение памяти будет успешным
    }

    // 2. Выделение еще одного блока памяти
    printf("Test 2: Allocating 200 bytes\n");
    void* p2 = my_malloc(200); // Выделяем 200 байт
    if (p2 != NULL) {
        printf("Success: Memory allocated at address: %p\n", p2); // Успешное выделение, адрес не NULL
    } else {
        printf("Fail: Failed to allocate memory.\n"); // Ожидаем, что выделение памяти будет успешным
    }

    // 3. Освобождение памяти
    printf("Test 3: Freeing 100 bytes\n");
    my_free(p1);  // Освобождение первого блока
    printf("Success: Memory freed at address: %p\n", p1); // Ожидаем, что память освобождена без ошибок

    // 4. Перевыделение памяти (увеличение размера)
    printf("Test 4: Reallocating memory to 300 bytes\n");
    p2 = my_realloc(p2, 300); // Перевыделяем память для p2 на 300 байт
    if (p2 != NULL) {
        printf("Success: Memory reallocated at address: %p\n", p2); // Успешное перевыделение, адрес не NULL
    } else {
        printf("Fail: Failed to reallocate memory.\n"); // Ожидаем, что перевыделение памяти будет успешным
    }

    // 5. Перевыделение памяти (уменьшение размера)
    printf("Test 5: Reallocating memory to 100 bytes\n");
    p2 = my_realloc(p2, 100); // Уменьшаем размер p2 до 100 байт
    if (p2 != NULL) {
        printf("Success: Memory reallocated at address: %p\n", p2); // Успешное перевыделение, адрес не NULL
    } else {
        printf("Fail: Failed to reallocate memory.\n"); // Ожидаем, что перевыделение памяти будет успешным
    }

    // 6. Попытка выделить слишком много памяти
    printf("Test 6: Allocating 2000 bytes (more than buffer size)\n");
    void* p3 = my_malloc(2000); // Пытаемся выделить 2000 байт
    if (p3 == NULL) {
        printf("Success: Failed to allocate 2000 bytes as expected.\n"); // Ожидаем, что выделение не удастся
    } else {
        printf("Fail: Unexpectedly allocated 2000 bytes at address: %p\n", p3); // Это не должно было произойти
    }

    // 7. Освобождение уже освобожденного блока
    printf("Test 7: Freeing already freed memory\n");
    my_free(p1);  // Попытка освободить p1, который уже был освобожден
    printf("Success: Memory freed at address: %p\n", p1); // Ожидаем, что освобождение пройдет без ошибок

    // 8. Попытка перевыделить освобожденный блок
    printf("Test 8: Reallocating already freed memory\n");
    void* p4 = my_realloc(p1, 50);  // p1 уже был освобожден
    if (p4 == NULL) {
        printf("Success: Failed to reallocate freed memory as expected.\n"); // Ожидаем, что перевыделение не удастся
    } else {
        printf("Fail: Unexpectedly reallocated freed memory at address: %p\n", p4); // Это не должно было произойти
    }

    // 9. Работа с NULL-указателями
    printf("Test 9: Freeing NULL pointer\n");
    my_free(NULL); // Освобождение NULL указателя
    printf("Success: Freed NULL pointer as expected.\n");

    // 10. Завершение тестов
    printf("All tests completed.\n");

    return 0;
}
