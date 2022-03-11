# CMakeList.txt: arquivo de projeto de CMake de nível superior, fazer configuração global
# e inclua subprojetos aqui.
#
cmake_minimum_required (VERSION 3.8)

project ("InfinityCmake")

# Inclua subprojetos.
add_subdirectory ("InfinityPcre")
add_subdirectory ("H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86" "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86")

# create executable ${APPLICATION_NAME}
