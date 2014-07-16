project(SIOX)

include_directories(
	${SIOX_INCLUDE_DIRS}
)

add_library(
	feign_siox-provider
	feign_siox-provider.cpp
)

target_link_libraries(
	feign_siox-provider
	siox-tools-TraceReader
)
