include(CTest)
enable_testing()

add_test(NAME help COMMAND ${PROJECT_NAME} "-h")
set_tests_properties(help PROPERTIES WILL_FAIL false)

add_test(NAME wrong_arg_1 COMMAND ${PROJECT_NAME})
set_tests_properties(wrong_arg_1 PROPERTIES WILL_FAIL true)

add_test(NAME wrong_arg_2 COMMAND ${PROJECT_NAME} "https://www.yahoo.com/news/rss" "-C")
set_tests_properties(wrong_arg_2 PROPERTIES WILL_FAIL true)

add_test(NAME wrong_arg_3 COMMAND ${PROJECT_NAME} "https://www.yahoo.com/news/rss" "-c")
set_tests_properties(wrong_arg_3 PROPERTIES WILL_FAIL true)

add_test(NAME atom_short COMMAND ${PROJECT_NAME} "https://xkcd.com/atom.xml")
set_tests_properties(atom_short PROPERTIES WILL_FAIL false)

add_test(NAME rss_short COMMAND ${PROJECT_NAME} "http://martinkocich.jecool.net/rss.xml")
set_tests_properties(rss_short PROPERTIES WILL_FAIL false)

set_tests_properties(PROPERTIES TIMEOUT 30)
