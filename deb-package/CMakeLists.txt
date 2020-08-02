project(packing)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Network)

message("[INFO] La raiz del proyecto es ${PROJECT_SOURCE_DIR}")

include_directories(${PROJECT_SOURCE_DIR}/include)

qt5_wrap_ui(UIS_HEADERS
        ui/Client.ui
)

# Agrego la carpeta con los headers autogenerados a los includes
message("Agregando '${PROJECT_BINARY_DIR}' como directorio de includes")
include_directories(${PROJECT_BINARY_DIR})

message("Headers generados: ${UIS_HEADERS}")

add_executable(test-client
  src/Client.cpp
  src/main.cpp
  ${UIS_HEADERS})

# Use the Widgets module from Qt 5.
target_link_libraries(test-client Qt5::Widgets Qt5::Network)
