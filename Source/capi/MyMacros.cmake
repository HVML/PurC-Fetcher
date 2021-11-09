macro(GENERATE_MSG_SOURCES _output_source _inputs)
    unset(_input_files)
    unset(_outputs)
    file(MAKE_DIRECTORY "${MESSAGES_DERIVED_SOURCES_DIR}")
    foreach (_file IN ITEMS ${_inputs})
        get_filename_component(_name ${_file} NAME_WE)
        list(APPEND _input_files ${CAPI_DIR}/${_file}.msg.in)
        list(APPEND _outputs
            ${MESSAGES_DERIVED_SOURCES_DIR}/fetcher-${_name}.c
            ${MESSAGES_DERIVED_SOURCES_DIR}/fetcher-${_name}.h
        )
        list(APPEND ${_output_source} ${MESSAGES_DERIVED_SOURCES_DIR}/fetcher-${_name}.c)
    endforeach ()

    set(message_header_file ${PURCFETCHER_DIR}/messages/MessageNames.h)
    add_custom_command(
        OUTPUT
            ${_outputs}
        MAIN_DEPENDENCY ${TOOLS_DIR}/Scripts/generate-msg.py
        DEPENDS
            ${TOOLS_DIR}/Scripts/generator/__init__.py
            ${TOOLS_DIR}/Scripts/generator/model.py
            ${TOOLS_DIR}/Scripts/generator/msg.py
            ${_input_files}
            COMMAND ${CMAKE_COMMAND} -E copy ${message_header_file} ${MESSAGES_DERIVED_SOURCES_DIR}/
            COMMAND ${PYTHON_EXECUTABLE} ${TOOLS_DIR}/Scripts/generate-msg.py ${CAPI_DIR} ${_inputs}
        WORKING_DIRECTORY ${MESSAGES_DERIVED_SOURCES_DIR}
        VERBATIM
    )
endmacro()
