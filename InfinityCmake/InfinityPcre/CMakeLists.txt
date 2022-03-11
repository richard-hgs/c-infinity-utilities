# CMakeList.txt: projeto de CMake para InfinityPcre, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

# Adicione a origem ao executável deste projeto.
add_library(${PROJECT_NAME} INTERFACE)
# add_executable (InfinityPcre "InfinityPcre.cpp" "InfinityPcre.h")

# TARGET_LINK_LIBRARIES(pcre2-16-x86)

# TODO: Adicione testes e instale destinos, se necessário.
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
            "Pcre2-10.23-x86/src"
            "Pcre2-10.23-x86")


# add_library(pcre2 INTERFACE)
#if(MSVC)
#    add_custom_target(pcre2.headers SOURCES ../../pcre2.h)
#endif()

#target_include_directories(pcre2 INTERFACE
#    "../../Pcre2-x86"
#)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_14)


#LINK_DIRECTORIES(../../Debug)
#TARGET_LINK_LIBRARIES(${PROJECT_NAME} INTERFACE pcre2)