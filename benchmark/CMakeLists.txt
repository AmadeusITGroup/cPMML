file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(benchmark ALL)

add_executable(model_benchmark.exe model_benchmark.cc)
target_include_directories(model_benchmark.exe PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(model_benchmark.exe PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(model_benchmark.exe PRIVATE ${PROJECT_SOURCE_DIR}/third_party)
target_link_libraries(model_benchmark.exe ${PROJECT_NAME} ${ADDITIONAL_LINK_LIBRARIES})
macro(add_model_benchmark TARGET DATASET)
    add_custom_target(${TARGET}_benchmark_run
            COMMAND model_benchmark.exe data/model/${TARGET}.xml data/dataset/${DATASET}.csv
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Benchmarking ${TARGET}..."
            )
    add_dependencies(benchmark ${TARGET}_benchmark_run)
endmacro()

add_model_benchmark(AuditBinaryReg Audit)
add_model_benchmark(AuditRandomForest Audit)
add_model_benchmark(AuditTree Audit)
add_model_benchmark(ensemble_audit_dectree Audit)
add_model_benchmark(ensemble_iris_dectree Iris)
add_model_benchmark(IrisLinearReg Iris)
add_model_benchmark(IrisMultinomReg Iris)
add_model_benchmark(IrisRandomForest Iris)
add_model_benchmark(IrisTree Iris)
add_model_benchmark(single_audit_dectree Audit)
add_model_benchmark(single_iris_dectree Iris)

#add_model_benchmark(HousingGBTlassifier Housing)
#add_model_benchmark(HousingRFClassifier Housing)
#add_model_benchmark(HousingLRClassifier Housing)
#add_model_benchmark(HousingGBTlassifier_PCA Housing)
#add_model_benchmark(HousingRFClassifier_PCA Housing)
#add_model_benchmark(HousingLRClassifier_PCA Housing)
#add_model_benchmark(HousingGBTRegressor Housing)
#add_model_benchmark(HousingRFRegressor Housing)
#add_model_benchmark(HousingLinearRegressor Housing)
#add_model_benchmark(HousingGBTRegressor_PCA Housing)
#add_model_benchmark(HousingRFRegressor_PCA Housing)
#add_model_benchmark(HousingLinearRegressor_PCA Housing)