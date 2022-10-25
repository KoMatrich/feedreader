include(CTest)
enable_testing()

add_test(NAME help COMMAND ${PROJECT_NAME} "-h")
set_tests_properties(help PROPERTIES WILL_FAIL false TIMEOUT 5)

add_test(NAME wrong_arg_1 COMMAND ${PROJECT_NAME})
set_tests_properties(wrong_arg_1 PROPERTIES WILL_FAIL true TIMEOUT 5)
add_test(NAME wrong_arg_2 COMMAND ${PROJECT_NAME} "https://www.yahoo.com/news/rss" "-C")
set_tests_properties(wrong_arg_2 PROPERTIES WILL_FAIL true TIMEOUT 5)
add_test(NAME wrong_arg_3 COMMAND ${PROJECT_NAME} "https://www.yahoo.com/news/rss" "-c")
set_tests_properties(wrong_arg_3 PROPERTIES WILL_FAIL true TIMEOUT 5)

add_test(NAME atom_fit COMMAND ${PROJECT_NAME}
	"http://www.fit.vutbr.cz/news/news-rss.php")
set_tests_properties(atom_fit PROPERTIES WILL_FAIL false TIMEOUT 5)

add_test(NAME atom_xkcd COMMAND ${PROJECT_NAME}
	"https://xkcd.com/atom.xml")
set_tests_properties(atom_xkcd PROPERTIES WILL_FAIL false TIMEOUT 5)

add_test(NAME atom_xkcd-T COMMAND ${PROJECT_NAME} "-T"
	"https://xkcd.com/atom.xml")
set_tests_properties(atom_xkcd-T PROPERTIES WILL_FAIL false TIMEOUT 5)

add_test(NAME atom_xkcd-Tau COMMAND ${PROJECT_NAME} "-T" "-a" "-u"
	"https://xkcd.com/atom.xml")
set_tests_properties(atom_xkcd-Tau PROPERTIES WILL_FAIL false TIMEOUT 5)

add_test(NAME rss_jecool_not_secured COMMAND ${PROJECT_NAME}
	"http://martinkocich.jecool.net/rss.xml")
set_tests_properties(rss_jecool_not_secured PROPERTIES WILL_FAIL false TIMEOUT 5)

add_test(NAME atom_wiki COMMAND ${PROJECT_NAME}
	"https://en.wikipedia.org/w/api.php?hidebots=1&days=7&limit=50&hidewikidata=1&action=feedrecentchanges&feedformat=atom")
set_tests_properties(atom_wiki PROPERTIES WILL_FAIL false TIMEOUT 5)

add_test(NAME atom_theregister COMMAND ${PROJECT_NAME}
	"https://www.theregister.co.uk/headlines.atom")
set_tests_properties(atom_theregister PROPERTIES WILL_FAIL false TIMEOUT 5)

add_test(NAME atom_theregister_data_center COMMAND ${PROJECT_NAME}
	"https://www.theregister.co.uk/data_centre/headlines.atom")
set_tests_properties(atom_theregister_data_center PROPERTIES WILL_FAIL false TIMEOUT 5)

add_test(NAME atom_what-if COMMAND ${PROJECT_NAME}
	"https://what-if.xkcd.com/feed.atom")
set_tests_properties(atom_what-if PROPERTIES WILL_FAIL false TIMEOUT 5)

add_test(NAME wrong_cert COMMAND ${PROJECT_NAME} "-C" "/dev/null"
	"https://what-if.xkcd.com/feed.atom")
set_tests_properties(atom_what-if PROPERTIES WILL_FAIL false TIMEOUT 5)
