test_development = executable(
    'test_development',
    src_files + [files('dev_main.cpp')],
    cpp_args: [],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)



n_array = executable(
    'n_array',
    src_files + [files('n_array.cpp')],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)
test('n_array', n_array, env : [ 'BOOST_TEST_LOG_LEVEL=message'])

n_incomplete = executable(
    'n_incomplete',
    src_files + [files('n_incomplete.cpp')],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)
test('n_incomplete', n_incomplete, env : [ 'BOOST_TEST_LOG_LEVEL=message'])

n_number = executable(
    'n_number',
    src_files + [files('n_number.cpp')],
    dependencies: [
        meson.get_compiler('cpp').find_library('boost_unit_test_framework')
    ],
    include_directories: inc_directory)
test('n_number', n_number, env : [ 'BOOST_TEST_LOG_LEVEL=message'])
