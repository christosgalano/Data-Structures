# Add subdirectories
add_subdirectory(test_list)
add_subdirectory(test_vector)
add_subdirectory(test_pq)
add_subdirectory(test_map)
add_subdirectory(test_trie)
add_subdirectory(test_graph)
add_subdirectory(test_bst)

# Add tests
add_test(NAME Test_Map COMMAND test_map)
add_test(NAME Test_Trie COMMAND test_trie)
add_test(NAME Test_Graph COMMAND test_graph)
add_test(NAME Test_Vector COMMAND test_vector)
add_test(NAME Test_Linked_List COMMAND test_list)
add_test(NAME Test_Priority_Queue COMMAND test_pq)
add_test(NAME Test_Binary_Search_Tree COMMAND test_bst)
