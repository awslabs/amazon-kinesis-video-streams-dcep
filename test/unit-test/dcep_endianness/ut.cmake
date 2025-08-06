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

# Create endianness test
set(utest_name "${project_name}_utest")
set(utest_source "${project_name}/${project_name}_utest.c")

create_test(${utest_name}
            ${utest_source}
            "${utest_link_list}"
            "${utest_dep_list}"
            "${test_include_directories}"
        )
