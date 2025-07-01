cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("19")

add_executable (19 "SSSMatrix.cpp" "SSSMatrix.h" "main.cpp")

# Тесты
add_executable(test_constructor tests/test_constructor.cpp SSSMatrix.cpp)
add_executable(test_input tests/test_input.cpp SSSMatrix.cpp)
add_executable(test_sum tests/test_sum.cpp SSSMatrix.cpp)
add_executable(test_print tests/test_print.cpp SSSMatrix.cpp)
add_executable(test_indices tests/test_indices.cpp SSSMatrix.cpp)
add_executable(stress_test tests/stress_test.cpp SSSMatrix.cpp)
add_executable(load_test tests/load_test.cpp SSSMatrix.cpp)
add_executable(stability_test tests/stability_test.cpp SSSMatrix.cpp)

# Добавляем тесты в CTest
enable_testing()
add_test(NAME ConstructorTest COMMAND test_constructor)
add_test(NAME InputTest COMMAND test_input)
add_test(NAME SumTest COMMAND test_sum)
add_test(NAME PrintTest COMMAND test_print)
add_test(NAME IndicesTest COMMAND test_indices)
add_test(NAME StressTest COMMAND stress_test)
add_test(NAME LoadTest COMMAND load_test)
add_test(NAME StabilityTest COMMAND stability_test)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 19 PROPERTY CXX_STANDARD 20)
endif()