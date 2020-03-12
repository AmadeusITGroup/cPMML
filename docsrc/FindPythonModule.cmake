set(FIND_PYTHON_TRIED FALSE)

function(find_python_module module)
    if(NOT FIND_PYTHON_TRIED)
        find_package(PythonInterp)
        set(FIND_PYTHON_TRIED TRUE)
    endif(NOT FIND_PYTHON_TRIED)

    if(PYTHONINTERP_FOUND)
            string(TOUPPER ${module} module_upper)
            if(NOT ${module_upper}_FOUND)
                execute_process(COMMAND "${PYTHON_EXECUTABLE}" "-c" "import ${module}; print(${module}.__version__)"
                        RESULT_VARIABLE _${module}_found
                        OUTPUT_VARIABLE _${module}_version
                        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
                execute_process(COMMAND "${PYTHON_EXECUTABLE}" "-c" "import ${module}; print(${module}.__file__[:-12])"
                        RESULT_VARIABLE _${module}_found
                        OUTPUT_VARIABLE _${module}_location
                        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
                if(NOT _${module}_found)
                    set(${module_upper}_FOUND TRUE CACHE STRING "${module} Python module")
                    set(${module_upper}_VERSION "${_${module}_version}\" at \"${_${module}_location}" CACHE STRING "${module} Python module")
                endif(NOT _${module}_found)
            endif(NOT ${module_upper}_FOUND)
            find_package_handle_standard_args(${module}
                    FOUND_VAR ${module_upper}_FOUND
                    REQUIRED_VARS ${module_upper}_FOUND ${module_upper}_VERSION
                    VERSION_VAR ${module_upper}_VERSION)
    endif(PYTHONINTERP_FOUND)
endfunction(find_python_module)