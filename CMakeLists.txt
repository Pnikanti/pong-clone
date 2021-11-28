cmake_minimum_required (VERSION 3.8)
project (pong-clone VERSION 0.1)

set(SOURCES
	 ${PROJECT_SOURCE_DIR}/src/context.cpp
	 ${PROJECT_SOURCE_DIR}/src/physics.cpp
	 ${PROJECT_SOURCE_DIR}/src/graphics.cpp
	 ${PROJECT_SOURCE_DIR}/src/entitymanager.cpp
	 ${PROJECT_SOURCE_DIR}/src/entity.cpp
	 ${PROJECT_SOURCE_DIR}/src/shader.cpp
	 ${PROJECT_SOURCE_DIR}/src/camera.cpp
	 ${PROJECT_SOURCE_DIR}/src/input.cpp
	 ${PROJECT_SOURCE_DIR}/src/game.cpp
	 ${PROJECT_SOURCE_DIR}/src/log.cpp
	 ${PROJECT_SOURCE_DIR}/src/gui.cpp
	 ${PROJECT_SOURCE_DIR}/src/guimanager.cpp
)

set(IMGUI_SOURCES
	${PROJECT_SOURCE_DIR}/dependencies/imgui/lib/imgui.cpp
	${PROJECT_SOURCE_DIR}/dependencies/imgui/lib/imgui_draw.cpp
	${PROJECT_SOURCE_DIR}/dependencies/imgui/lib/imgui_impl_glfw.cpp
	${PROJECT_SOURCE_DIR}/dependencies/imgui/lib/imgui_impl_opengl3.cpp
	${PROJECT_SOURCE_DIR}/dependencies/imgui/lib/imgui_tables.cpp
	${PROJECT_SOURCE_DIR}/dependencies/imgui/lib/imgui_widgets.cpp
)
set(BOX2D_SOURCES
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_broad_phase.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_chain_shape.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_circle_shape.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_collide_circle.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_collide_edge.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_collide_polygon.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_collision.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_distance.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_dynamic_tree.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_edge_shape.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_polygon_shape.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/collision/b2_time_of_impact.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/common/b2_block_allocator.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/common/b2_draw.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/common/b2_math.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/common/b2_settings.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/common/b2_stack_allocator.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/common/b2_timer.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_body.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_chain_circle_contact.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_chain_circle_contact.h
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_chain_polygon_contact.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_chain_polygon_contact.h
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_circle_contact.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_circle_contact.h
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_contact.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_contact_manager.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_contact_solver.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_contact_solver.h
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_distance_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_edge_circle_contact.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_edge_circle_contact.h
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_edge_polygon_contact.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_edge_polygon_contact.h
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_fixture.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_friction_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_gear_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_island.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_island.h
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_motor_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_mouse_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_polygon_circle_contact.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_polygon_circle_contact.h
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_polygon_contact.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_polygon_contact.h
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_prismatic_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_pulley_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_revolute_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_weld_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_wheel_joint.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_world.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/dynamics/b2_world_callbacks.cpp
	${PROJECT_SOURCE_DIR}/dependencies/box2d/src/rope/b2_rope.cpp
)

find_package(OpenGL REQUIRED)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
add_executable (${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES} ${BOX2D_SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
add_definitions(-DGLEW_STATIC)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
	add_definitions(-DDEBUG)
else()
	set_target_properties(${PROJECT_NAME} PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
endif()

target_include_directories(${PROJECT_NAME}
	PUBLIC ${OPENGL_INCLUDE_DIRS}
	PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/glfw/include
	PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/glew/include
	PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/glm/include
	PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/spdlog/include
	PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/imgui/include
	PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/box2d/include
	PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/robin_hood/include
	PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/box2d/src
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC ${OPENGL_LIBRARIES}
	PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/glfw/lib/glfw3.lib
	PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/glew/lib/glew32s.lib
)