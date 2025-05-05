cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("20")

add_executable (20 "19.cpp" "19.h" "main.cpp")

# Тестовые исполняемые файлы
add_executable(test_constructor tests/test_constructor.cpp 19.cpp)
add_executable(test_read tests/test_read.cpp 19.cpp)
add_executable(test_sort tests/test_sort.cpp 19.cpp)
add_executable(test_reconstruct tests/test_reconstruct.cpp 19.cpp)
add_executable(stress_test tests/stress_test.cpp 19.cpp)
add_executable(load_test tests/load_test.cpp 19.cpp)
add_executable(stability_test tests/stability_test.cpp 19.cpp)
add_executable(white_box tests/white_box.cpp 19.cpp)

enable_testing()
# Добавляем тесты в CTest
enable_testing()
add_test(NAME ConstructorTest COMMAND test_constructor)
add_test(NAME ReadTest COMMAND test_read)
add_test(NAME SortTest COMMAND test_sort)
add_test(NAME ReconstructTest COMMAND test_reconstruct)
add_test(NAME StressTest COMMAND stress_test)
add_test(NAME LoadTest COMMAND load_test)
add_test(NAME StabilityTest COMMAND stability_test)
add_test(NAME WhiteBox COMMAND white_box)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 20 PROPERTY CXX_STANDARD 20)
endif()

