cmake_minimum_required (VERSION 2.8.12)

project (genmac C)

macro (install_path _DIR _DEFAULT)
  if (NOT ${_DIR})
    set (${_DIR} ${_DEFAULT})
  endif ()
endmacro ()

install_path (BINDIR bin)
install_path (MANDIR man)
install_path (MAN1DIR ${MANDIR}/man1)

# TODO: add -Wall -Wshadow to CFLAGS, if it is not already defined
# TODO: add -std=c89 -D_GNU_SOURCE to FLAGS (regardless of its current value)

find_program (ASCIIDOC asciidoc DOC "Full path to AsciiDoc executable; required for building the manual page")
find_program (XMLTO xmlto DOC "Full path to xmlto executable; required for building the manual page")

# TODO: check what cmake does with clean/distclean

# TODO: check whether cpack can be used instead of my own dist target

# TODO: add custom command/target indent

# Executable
add_executable(genmac genmac.c)
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
