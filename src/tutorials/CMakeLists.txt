
function(create_project_from_sources TUTORIAL_DIR)
  file(GLOB SOURCE
    "${TUTORIAL_DIR}/*.h"
    "${TUTORIAL_DIR}/*.cpp"
  )
  file(GLOB SHADERS
    "${TUTORIAL_DIR}/*.vs"
    "${TUTORIAL_DIR}/*.fs"
    "${TUTORIAL_DIR}/*.tcs"
    "${TUTORIAL_DIR}/*.tes"
    "${TUTORIAL_DIR}/*.gs"
    "${TUTORIAL_DIR}/*.cs"
  )
	file(GLOB DLLS)
  
  get_filename_component(TUTORIAL_NAME ${TUTORIAL_DIR} NAME)

  add_executable(${TUTORIAL_NAME} ${SOURCE} ${SHADERS} ${COMMON})
  target_link_libraries(${TUTORIAL_NAME} ${LIBS})

  set_target_properties(${TUTORIAL_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/tutorials/${TUTORIAL_NAME})

  foreach(SHADER ${SHADERS})
    add_custom_command(TARGET ${TUTORIAL_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${TUTORIAL_NAME}>)
  endforeach(SHADER)

  foreach(DLL ${DLLS})
    add_custom_command(TARGET ${TUTORIAL_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:${TUTORIAL_NAME}>)
  endforeach(DLL)
endfunction()


file(GLOB TUTORIAL_DIRS LIST_DIRECTORIES true "*")

foreach(TUTORIAL_DIR ${TUTORIAL_DIRS})
  IF(IS_DIRECTORY ${TUTORIAL_DIR})
    create_project_from_sources(${TUTORIAL_DIR})
  ENDIF()
endforeach(TUTORIAL_DIR)
