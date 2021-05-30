test_development = executable(
    'test_development',
    src_files + [files('dev_main.cpp')],
    cpp_args: [],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)



json_test_suite_array_failures = executable(
    'json_test_suite_array_failures',
    src_files + [files('json_test_suite_array_failures.cpp')],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)
test('json_test_suite_array_failures', json_test_suite_array_failures, env : [ 'BOOST_TEST_LOG_LEVEL=message'])


json_test_suite_incomplete_failures = executable(
    'json_test_suite_incomplete_failures',
    src_files + [files('json_test_suite_incomplete_failures.cpp')],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)
test('json_test_suite_incomplete_failures', json_test_suite_incomplete_failures, env : [ 'BOOST_TEST_LOG_LEVEL=message'])


json_test_suite_number_failures = executable(
    'json_test_suite_number_failures',
    src_files + [files('json_test_suite_number_failures.cpp')],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)
test('json_test_suite_number_failures', json_test_suite_number_failures, env : [ 'BOOST_TEST_LOG_LEVEL=message'])


json_test_suite_object_failures = executable(
    'json_test_suite_object_failures',
    src_files + [files('json_test_suite_object_failures.cpp')],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)
test('json_test_suite_object_failures', json_test_suite_object_failures, env : [ 'BOOST_TEST_LOG_LEVEL=message'])


json_test_suite_string_failures = executable(
    'json_test_suite_string_failures',
    src_files + [files('json_test_suite_string_failures.cpp')],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)
test('json_test_suite_string_failures', json_test_suite_string_failures, env : [ 'BOOST_TEST_LOG_LEVEL=message'])


json_test_suite_structure_failures = executable(
    'json_test_suite_structure_failures',
    src_files + [files('json_test_suite_structure_failures.cpp')],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)
test('json_test_suite_structure_failures', json_test_suite_structure_failures, env : [ 'BOOST_TEST_LOG_LEVEL=message'])


json_test_suite_array_successes = executable(
    'json_test_suite_array_successes',
    src_files + [files('json_test_suite_array_successes.cpp')],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)
test('json_test_suite_array_successes', json_test_suite_array_successes, env : [ 'BOOST_TEST_LOG_LEVEL=message'])
