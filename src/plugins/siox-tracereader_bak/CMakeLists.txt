project(SIOX)

include_directories(
	${SIOX_INCLUDE_DIRS}
)

add_executable(
	feign-TraceReader-test
	feign_siox-provider.cpp
)


add_library(
	feign_siox-provider
	feign_siox-provider.cpp
)


target_link_libraries(
	feign-TraceReader-test
	siox-tools-TraceReader
)


target_link_libraries(
	feign_siox-provider
	siox-tools-TraceReader
)

symlink(activities.dat activities.dat)
symlink(association.dat association.dat)
symlink(ontology.dat ontology.dat)
symlink(system-info.dat system-info.dat)

add_test(tools-TraceReader-test tools-TraceReader-test)
