cmake_minimum_required(VERSION 3.5)

project(genwopl LANGUAGES C CXX)

find_package(Qt5Core CONFIG REQUIRED)

add_executable(genwopl
  "sources/genwopl.cc"
  "sources/common.cpp"
  "sources/bank.cpp"
  "sources/opl/measurer.cpp"
  "sources/opl/chips/dosbox_opl3.cpp"
  "sources/opl/chips/dosbox/dbopl.cpp"
  "sources/ini/ini_processing.cpp"
  "sources/FileFormats/ffmt_base.cpp"
  "sources/FileFormats/ffmt_factory.cpp"
  "sources/FileFormats/format_adlib_bnk.cpp"
  "sources/FileFormats/format_adlib_tim.cpp"
  "sources/FileFormats/format_ail2_gtl.cpp"
  "sources/FileFormats/format_apogeetmb.cpp"
  "sources/FileFormats/format_bisqwit.cpp"
  "sources/FileFormats/format_cmf_importer.cpp"
  "sources/FileFormats/format_dmxopl2.cpp"
  "sources/FileFormats/format_flatbuffer_opl3.cpp"
  "sources/FileFormats/format_imf_importer.cpp"
  "sources/FileFormats/format_junlevizion.cpp"
  "sources/FileFormats/format_rad_importer.cpp"
  "sources/FileFormats/format_sb_ibk.cpp"
  "sources/FileFormats/format_wohlstand_opl3.cpp"
  "sources/FileFormats/wopl/wopl_file.c")

target_link_libraries(genwopl Qt5::Core)
