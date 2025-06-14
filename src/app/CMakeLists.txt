qt_add_executable(telecom_tree main.cpp)
target_link_libraries(telecom_tree PRIVATE
					  Qt6::Core
					  $<$<CONFIG:Debug>:Qt6::CorePrivate>
					  Qt6::Widgets
					  $<$<CONFIG:Debug>:Qt6::WidgetsPrivate>
					  Qt6::Sql
)
set_target_properties(telecom_tree PROPERTIES WIN32_EXECUTABLE ON)

set(STYLE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/style")
set(STYLE_DEST_DIR "${CMAKE_BINARY_DIR}/style")

set(ICONS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/icons")
set(ICONS_DEST_DIR "${CMAKE_BINARY_DIR}/icons")

set(DB_FILE "${CMAKE_SOURCE_DIR}/OM_system.db")
set(DB_DEST_DIR $<TARGET_FILE_DIR:telecom_tree>)

add_custom_command(
	TARGET telecom_tree POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${STYLE_SOURCE_DIR}"
	"${STYLE_DEST_DIR}"
)

add_custom_command(
	TARGET telecom_tree POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${ICONS_SOURCE_DIR}"
	"${ICONS_DEST_DIR}"
)

add_custom_command(TARGET telecom_tree POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   "${DB_FILE}"
				   "${DB_DEST_DIR}"
)