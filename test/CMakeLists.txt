file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(unit_tests ALL)

add_executable(model_tester.exe model_tester.cc)
target_include_directories(model_tester.exe PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(model_tester.exe PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(model_tester.exe PRIVATE ${PROJECT_SOURCE_DIR}/third_party)
target_link_libraries(model_tester.exe ${PROJECT_NAME} ${ADDITIONAL_LINK_LIBRARIES})
add_dependencies(unit_tests model_tester.exe)
macro(add_model_test TARGET)
    add_test(${TARGET} model_tester.exe data/model/${TARGET}.zip data/dataset/${TARGET}.csv)
endmacro()

#add_model_test(AuditBinaryReg) # original prediction not available
add_model_test(AuditRandomForest)
add_model_test(AuditTree)
add_model_test(ensemble_audit_dectree)
add_model_test(ensemble_iris_dectree)
add_model_test(IrisLinearReg)
add_model_test(IrisMultinomReg)
add_model_test(IrisRandomForest)
add_model_test(IrisTree)
add_model_test(single_audit_dectree)
add_model_test(single_iris_dectree)

add_model_test(HousingGBTClassifier)
add_model_test(HousingRFClassifier)
add_model_test(HousingLRClassifier)
add_model_test(HousingLinearRegressor)

add_model_test(HousingGBTClassifier_PCA)
add_model_test(HousingRFClassifier_PCA)
add_model_test(HousingLRClassifier_PCA)
add_model_test(HousingGBTRegressor_PCA)
add_model_test(HousingRFRegressor_PCA)
add_model_test(HousingLinearRegressor_PCA)

add_custom_command(
        TARGET unit_tests
        COMMENT "Running unit tests..."
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
)