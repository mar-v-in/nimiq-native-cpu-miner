cmake_minimum_required(VERSION 3.9)
project(nimiq-miner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -g")

find_package(cpprestsdk REQUIRED)
find_package(Boost REQUIRED system program_options)

FUNCTION(PREPEND var prefix)
    SET(listVar "")
    FOREACH(f ${ARGN})
        LIST(APPEND listVar "${prefix}/${f}")
    ENDFOREACH(f)
    SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

set(CORE_NATIVE_DIR "./core/src/native")
set(CORE_NATIVE_FILES nimiq_native.c opt.c argon2.c core.c encoding.c blake2/blake2b.c)
PREPEND(CORE_NATIVE_FILES ${CORE_NATIVE_DIR} ${CORE_NATIVE_FILES})

add_executable(nimiq-miner main.cpp miner.cpp json_rpc_client.cpp core_native_miner.cpp ${CORE_NATIVE_FILES} minimal_core_native.c utils.cpp block_template.cpp get_work_provider.cpp get_work_provider.h get_block_template_provider.cpp get_block_template_provider.h provider.cpp provider.h)
add_definitions(-DEXCLUDE_NIMIQ_SHA -DARGON2_NO_THREADS)
target_include_directories(nimiq-miner PUBLIC ${CORE_NATIVE_DIR})
target_link_libraries(nimiq-miner PRIVATE cpprestsdk::cpprest ${Boost_SYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})
