cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Ecosystem Simulator")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (CMakeTarget "Commons.h" "Field.cpp" "Field.h"  "Creatures.cpp" "Creatures.h"  "Ecosystem Simulator.cpp" )


# TODO: Добавьте тесты и целевые объекты, если это необходимо.

# SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0")
include_directories(${SFML_DIR}/include)
link_directories(${SFML_DIR}/lib)

#add_executable(EcosystemSimulator "Ecosystem Simulator.cpp")
add_executable(SFML_Project gui/gui.cpp)

target_link_libraries(SFML_Project sfml-graphics-d sfml-window-d sfml-system-d)

set(DLL_FILES
        ${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0/bin/sfml-graphics-d-3.dll
        ${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0/bin/sfml-window-d-3.dll
        ${CMAKE_SOURCE_DIR}/libs/SFML-3.0.0/bin/sfml-system-d-3.dll
)

foreach(DLL ${DLL_FILES})
    add_custom_command(TARGET SFML_Project POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL} $<TARGET_FILE_DIR:SFML_Project>
            COMMENT "Copying ${DLL} to output directory"
    )
endforeach()


add_custom_command(TARGET SFML_Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SFML_DIR}/bin $<TARGET_FILE_DIR:SFML_Project>)

