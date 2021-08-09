add_library(EasyLoggingObjLib OBJECT easylogging++.cc)
set_property(TARGET EasyLoggingObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_compile_definitions(EasyLoggingObjLib PUBLIC ELPP_NO_DEFAULT_LOG_FILE)
install(FILES easylogging++.h DESTINATION include/easylogging)
