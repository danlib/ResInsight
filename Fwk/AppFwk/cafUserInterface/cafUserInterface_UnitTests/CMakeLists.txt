cmake_minimum_required (VERSION 2.8.12)

find_package(Qt5 CONFIG COMPONENTS Core)
if (Qt5Core_FOUND)
	find_package(Qt5 CONFIG REQUIRED Core Gui Widgets)
else()
	find_package(Qt4 COMPONENTS QtCore QtGui QtMain REQUIRED)
    include(${QT_USE_FILE})
endif(Qt5Core_FOUND)

project ( cafUserInterface_UnitTests )

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}   
)


set( PROJECT_FILES

	cafUserInterface_UnitTests.cpp 
	
	cafPdmUiTreeViewModelTest.cpp 
	cafPdmUiTreeSelectionQModelTest.cpp 

	gtest/gtest-all.cpp
)

# add the executable 
add_executable (${PROJECT_NAME}
	${PROJECT_FILES}
)

source_group("" FILES ${PROJECT_FILES})

if (Qt5Core_FOUND)
	set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets)
endif()

target_link_libraries ( ${PROJECT_NAME}
    cafUserInterface
    ${QT_LIBRARIES}
    ${THREAD_LIBRARY}
)

# Copy Qt Dlls
if (Qt5Core_FOUND)
    foreach (qtlib ${QT_LIBRARIES})
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${qtlib}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    	)
	endforeach(qtlib)
	# Copy Qt Dlls	
else()
	# Copy Qt Dlls
	if (MSVC)
	    set (QTLIBLIST QtCore QtGui QtOpenGl )
	    foreach (qtlib ${QTLIBLIST})
	        
	        # Debug
	        execute_process(COMMAND cmake -E copy_if_different ${QT_BINARY_DIR}/${qtlib}d4.dll ${CMAKE_BINARY_DIR}/Debug/${qtlib}d4.dll)
	        
	        # Release
	        execute_process(COMMAND cmake -E copy_if_different ${QT_BINARY_DIR}/${qtlib}4.dll ${CMAKE_BINARY_DIR}/Release/${qtlib}4.dll)
	    endforeach( qtlib )
	endif(MSVC)
endif(Qt5Core_FOUND)
