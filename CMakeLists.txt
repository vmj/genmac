cmake_minimum_required (VERSION 2.8.7)

project (genmac C)
set(PROJECT_VERSION 0.5)

macro (install_path _DIR _DEFAULT)
  if (NOT ${_DIR})
    set (${_DIR} ${_DEFAULT})
  endif ()
endmacro ()

install_path (BINDIR bin)
install_path (MANDIR man)
install_path (MAN1DIR ${MANDIR}/man1)

add_definitions(-Wall -Wshadow)
add_definitions(-std=c89 -D_GNU_SOURCE)

find_program (ASCIIDOC asciidoc DOC "Full path to AsciiDoc executable; required for building the manual page")
find_program (XMLTO xmlto DOC "Full path to xmlto executable; required for building the manual page")
find_program (MKDIR mkdir DOC "Full path to mkdir executable; required for building the source distribution")
find_program (CP cp DOC "Full path to cp executable; required for building the source distribution")
find_program (TAR tar DOC "Full path to tar executable; required for building the source distribution")

# TODO: add custom command/target indent

configure_file(
  ${PROJECT_SOURCE_DIR}/version.h.in
  ${PROJECT_BINARY_DIR}/version.h
  @ONLY)
include_directories(${PROJECT_BINARY_DIR})

# Executable
add_executable(genmac genmac.c version.h)
install (TARGETS genmac DESTINATION "${BINDIR}")

# Manual page
add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/genmac.1
  DEPENDS ${PROJECT_BINARY_DIR}/genmac.1.xml
  COMMAND ${XMLTO}
  ARGS -o ${PROJECT_BINARY_DIR} man ${PROJECT_BINARY_DIR}/genmac.1.xml
  COMMENT "Converting genmac.1.xml to genmac.1 (DocBook to troff)"
  )
add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/genmac.1.xml
  DEPENDS ${PROJECT_SOURCE_DIR}/genmac.1.txt
  COMMAND ${ASCIIDOC}
  ARGS -b docbook -d manpage -o ${PROJECT_BINARY_DIR}/genmac.1.xml ${PROJECT_SOURCE_DIR}/genmac.1.txt
  COMMENT "Converting genmac.1.txt to genmac.1.xml (AsciiDoc to DocBook)"
  )
add_custom_target(man ALL
  DEPENDS ${PROJECT_BINARY_DIR}/genmac.1
  )
install (FILES ${PROJECT_BINARY_DIR}/genmac.1 DESTINATION "${MAN1DIR}")

# HTML documentation
add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/index.html
  DEPENDS ${PROJECT_SOURCE_DIR}/README.adoc
  COMMAND ${ASCIIDOC}
  ARGS -b html -o ${PROJECT_BINARY_DIR}/index.html ${PROJECT_SOURCE_DIR}/README.adoc
  COMMENT "Converting README.adoc to index.html (AsciiDoc to HTML)"
  )
add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/genmac.1.html
  DEPENDS ${PROJECT_SOURCE_DIR}/genmac.1.txt
  COMMAND ${ASCIIDOC}
  ARGS -b html -d manpage -o ${PROJECT_BINARY_DIR}/genmac.1.html ${PROJECT_SOURCE_DIR}/genmac.1.txt
  COMMENT "Converting genmac.1.txt to genmac.1.html (AsciiDoc to HTML)"
  )
add_custom_target(html ALL
  DEPENDS ${PROJECT_BINARY_DIR}/index.html ${PROJECT_BINARY_DIR}/genmac.1.html
  )

# Source distribution
add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/genmac-${PROJECT_VERSION}.tar.gz
  COMMAND
    ${MKDIR}
  ARGS
    ${PROJECT_BINARY_DIR}/genmac-${PROJECT_VERSION}
  COMMAND
    ${CP}
  ARGS
    ${PROJECT_SOURCE_DIR}/LICENSE.txt
    ${PROJECT_SOURCE_DIR}/CMakeLists.txt
    ${PROJECT_SOURCE_DIR}/genmac.c
    ${PROJECT_SOURCE_DIR}/genmac.1.txt
    ${PROJECT_SOURCE_DIR}/README.adoc
    ${PROJECT_SOURCE_DIR}/version.h.in
    ${PROJECT_BINARY_DIR}/genmac-${PROJECT_VERSION}/
  COMMAND
    ${TAR}
  ARGS
    czf ${PROJECT_BINARY_DIR}/genmac-${PROJECT_VERSION}.tar.gz
    -C ${PROJECT_BINARY_DIR}
    genmac-${PROJECT_VERSION}
  )
add_custom_target(dist
  DEPENDS ${PROJECT_BINARY_DIR}/genmac-${PROJECT_VERSION}.tar.gz
  )
