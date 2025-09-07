# CMakeList.txt: проект CMake для Mod29_HW; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Mod29_HW "Mod29_HW.cpp" "Mod29_HW.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Mod29_HW PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
