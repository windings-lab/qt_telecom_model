file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADERS *.h)

target_sources(telecom_tree
			   PRIVATE ${SOURCES}
			   PUBLIC ${HEADERS}
)