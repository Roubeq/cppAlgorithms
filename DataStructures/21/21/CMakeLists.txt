# CMakeList.txt: проект CMake для 21; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("21")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (21 "SparseMatrix.cpp" "SparseMatrix.h" "main.cpp"  )

# Тестовые исполняемые файлы с добавлением исходников SparseMatrix
add_executable(test_constructor tests/test_constructor.cpp SparseMatrix.cpp SparseMatrix.h)
add_executable(test_read       tests/test_read.cpp       SparseMatrix.cpp SparseMatrix.h)
add_executable(test_rotate       tests/test_rotate.cpp       SparseMatrix.cpp SparseMatrix.h)
add_executable(test_print tests/test_print.cpp SparseMatrix.cpp SparseMatrix.h)
add_executable(test_unpack tests/test_unpack.cpp SparseMatrix.cpp SparseMatrix.h)
add_executable(stress_test tests/stress_test.cpp SparseMatrix.cpp SparseMatrix.h)
add_executable(load_test tests/load_test.cpp SparseMatrix.cpp SparseMatrix.h)
add_executable(stability_test tests/stability_test.cpp SparseMatrix.cpp SparseMatrix.h)

# Включаем тестирование
enable_testing()

# Добавляем тесты в CTest
add_test(NAME ConstructorTest  COMMAND test_constructor)
add_test(NAME ReadTest         COMMAND test_read)
add_test(NAME RotateTest         COMMAND test_rotate)
add_test(NAME UnpackTest  COMMAND test_unpack)
add_test(NAME PrintTest  COMMAND test_print)
add_test(NAME StressTest COMMAND stress_test)
add_test(NAME LoadTest COMMAND load_test)
add_test(NAME StabilityTest COMMAND stability_test)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 21 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
