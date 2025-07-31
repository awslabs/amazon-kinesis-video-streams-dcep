# Include filepaths for source and include.
include( ${MODULE_ROOT_DIR}/dcepFilePaths.cmake )

# ====================  Define your project name (edit) ========================
set( project_name "dcep_endianness" )

message( STATUS "${project_name}" )

# ================= Create the library under test here (edit) ==================

# List the files you would like to test here.
list(APPEND real_source_files
            ${DCEP_SOURCES}
        )
# List the directories the module under test includes.
list(APPEND real_include_directories
            ${DCEP_INCLUDE_PUBLIC_DIRS}
            ${MODULE_ROOT_DIR}/test/unit-test
            ${CMOCK_DIR}/vendor/unity/src
        )

# =====================  Create UnitTest Code here (edit)  =====================

# list the directories your test needs to include.
list(APPEND test_include_directories
            ${CMOCK_DIR}/vendor/unity/src
            ${DCEP_INCLUDE_PUBLIC_DIRS}
            ${MODULE_ROOT_DIR}/test/unit-test
        )

# =============================  (end edit)  ===================================

set(real_name "${project_name}_real")

create_real_library(${real_name}
                    "${real_source_files}"
                    "${real_include_directories}"
                    ""
        )

list(APPEND utest_link_list
            lib${real_name}.a
        )

list(APPEND utest_dep_list
            ${real_name}
        )

# Create test for little-endian path
# dcep_endianness_utest_le uses dcep_endianness/dcep_endianness_utest.c with FORCE_LITTLE_ENDIAN
set(utest_name_le "${project_name}_utest_le")
set(utest_source "${project_name}/${project_name}_utest.c")

# Create separate library for little-endian testing
set(real_name_le "${project_name}_real_le")
create_real_library(${real_name_le}
                    "${real_source_files}"
                    "${real_include_directories}"
                    ""
        )
target_compile_definitions(${real_name_le} PRIVATE FORCE_LITTLE_ENDIAN)

list(APPEND utest_link_list_le
            lib${real_name_le}.a
        )

list(APPEND utest_dep_list_le
            ${real_name_le}
        )

create_test(${utest_name_le}
            ${utest_source}
            "${utest_link_list_le}"
            "${utest_dep_list_le}"
            "${test_include_directories}"
        )
target_compile_definitions(${utest_name_le} PRIVATE FORCE_LITTLE_ENDIAN)

# Create test for big-endian path
# dcep_endianness_utest_be uses dcep_endianness/dcep_endianness_utest.c with FORCE_BIG_ENDIAN
set(utest_name_be "${project_name}_utest_be")

# Create separate library for big-endian testing
set(real_name_be "${project_name}_real_be")
create_real_library(${real_name_be}
                    "${real_source_files}"
                    "${real_include_directories}"
                    ""
        )
target_compile_definitions(${real_name_be} PRIVATE FORCE_BIG_ENDIAN)

list(APPEND utest_link_list_be
            lib${real_name_be}.a
        )

list(APPEND utest_dep_list_be
            ${real_name_be}
        )

create_test(${utest_name_be}
            ${utest_source}
            "${utest_link_list_be}"
            "${utest_dep_list_be}"
            "${test_include_directories}"
        )
target_compile_definitions(${utest_name_be} PRIVATE FORCE_BIG_ENDIAN)
