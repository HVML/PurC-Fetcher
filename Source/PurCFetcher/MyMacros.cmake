# Append the all C files in the specified directory list to the source list
macro(APPEND_ALL_SOURCE_FILES_IN_DIRLIST result)
    set(filelist "")
    foreach(module ${ARGN})
        list(APPEND filelist ${module}/*.c)
        list(APPEND filelist ${module}/*.cpp)
    endforeach()
    file(GLOB_RECURSE ${result} RELATIVE ${PURCFETCHER_DIR} ${filelist})
#    FOREACH(file ${${result}})
#        message(STATUS ${file})
#    ENDFOREACH()
    unset(filelist)
endmacro()

# Helper macro which wraps the generate-message-receiver.py script
#   _output_source is a list name which will contain generated sources.(eg. WebKit_SOURCES)
#   _inputs are messages.in files to generate.
macro(GENERATE_MESSAGE_SOURCES _output_source _inputs)
    unset(_input_files)
    unset(_outputs)
    file(MAKE_DIRECTORY "${MESSAGES_DERIVED_SOURCES_DIR}")
    foreach (_file IN ITEMS ${_inputs})
        get_filename_component(_name ${_file} NAME_WE)
        list(APPEND _input_files ${PURCFETCHER_DIR}/${_file}.messages.in)
        list(APPEND _outputs
            ${MESSAGES_DERIVED_SOURCES_DIR}/${_name}MessageReceiver.cpp
            ${MESSAGES_DERIVED_SOURCES_DIR}/${_name}Messages.h
            ${MESSAGES_DERIVED_SOURCES_DIR}/${_name}MessagesReplies.h
        )
        list(APPEND ${_output_source} ${MESSAGES_DERIVED_SOURCES_DIR}/${_name}MessageReceiver.cpp)
    endforeach ()
    list(APPEND ${_output_source} ${MESSAGES_DERIVED_SOURCES_DIR}/MessageNames.cpp)

    set(message_header_file ${PURCFETCHER_DIR}/messages/MessageNames.h)
    set(message_source_file ${PURCFETCHER_DIR}/messages/MessageNames.cpp)
    add_custom_command(
        OUTPUT
            ${MESSAGES_DERIVED_SOURCES_DIR}/MessageNames.cpp
            ${MESSAGES_DERIVED_SOURCES_DIR}/MessageNames.h
            ${_outputs}
        MAIN_DEPENDENCY ${TOOLS_DIR}/Scripts/generate-message-receiver.py
        DEPENDS
            ${TOOLS_DIR}/Scripts/generator/__init__.py
            ${TOOLS_DIR}/Scripts/generator/messages.py
            ${TOOLS_DIR}/Scripts/generator/model.py
            ${TOOLS_DIR}/Scripts/generator/parser.py
            ${_input_files}
            COMMAND ${PYTHON_EXECUTABLE} ${TOOLS_DIR}/Scripts/generate-message-receiver.py ${PURCFETCHER_DIR} ${_inputs}
            COMMAND ${CMAKE_COMMAND} -E copy ${message_header_file} ${MESSAGES_DERIVED_SOURCES_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy ${message_source_file} ${MESSAGES_DERIVED_SOURCES_DIR}/
        WORKING_DIRECTORY ${MESSAGES_DERIVED_SOURCES_DIR}
        VERBATIM
    )
endmacro()

