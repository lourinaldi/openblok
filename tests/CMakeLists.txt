set(TEST_SRC
	# test_GraphicsContext.cpp
	test_Color.cpp
	test_Piece.cpp
	test_Transition.cpp
	test_Well.cpp
	test_WellTSpin.cpp
	test_Well_TGM.cpp

	# TestUtils.cpp
	main.cpp
)

set(TEST_H
	# TestUtils.h
)

add_executable(openblok_test ${TEST_SRC} ${TEST_H})

target_link_libraries(openblok_test UnitTest++)
target_link_libraries(openblok_test module_system)
target_link_libraries(openblok_test module_game)


# use custom std::make_unique if necessary
if (NOT CXX_HAS_MAKEUNIQUE)
    target_compile_definitions(openblok_test PRIVATE NEED_MAKEUNIQUE)
endif()
