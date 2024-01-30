cmake_minimum_required(VERSION 3.9.0)

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")

set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_SUPPRESS_REGENERATION TRUE)
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "")

get_filename_component(RootDir "${CMAKE_CURRENT_LIST_DIR}" PATH)
get_filename_component(RootDir "${RootDir}" PATH)

add_definitions(-D_AFXDLL)
add_definitions(-D_WIN32_WINNT=_WIN32_WINNT_MAXVER)
set(CMAKE_MFC_FLAG 2) #use mfc in sharred dll

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt.lib")

project(SQLite2mdb)

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/DBUtils/sqlite")

function(include_root_dir)
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
endfunction(include_root_dir)

function(set_project_compile_options_debugrelease)
    IF( ${WIN32} ) 		
        list(APPEND COMPILEOPT_DEBUG
            /Od           	# Enable basic run-time checks
            /MDd		  	# Multithreaded debug runtime
        )

        list(APPEND COMPILEOPT_RELEASE
            /O2           	# Optimize for maximum speed
            /MD           	# Multithreaded release runtime
            /Ob1		  	# Inline function expansion
            /GF			  	# Enable string pooling
        )
    ELSE()
        list(APPEND COMPILEOPT_DEBUG -g -fPIC -shared -std=c++20)
        list(APPEND COMPILEOPT_RELEASE -fPIC -shared -export-dynamic -O2 -std=c++20)
    ENDIF()
	
	target_compile_options(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:${COMPILEOPT_DEBUG}>)
	target_compile_options(${PROJECT_NAME} PUBLIC $<$<CONFIG:Release>:${COMPILEOPT_RELEASE}>)
endfunction(set_project_compile_options_debugrelease)

function(set_project_compile_options)
    IF( ${WIN32} ) 	
        list(APPEND COMPILEOPT
            /Gy			  	# Enable function level linking
            /Yu			  	# Use precompiled headers
            #/Zm800			# Precompiled header memory allocation limit. Disabled for #43854 - W40: /Zm800 # Precompiled header memory allocation limit => can be removed?
            /GR				# Enable run type information
            /std:c++20
            /JMC			# Enables Just My Code stepping, which brings better debuging experience reference #32706 for more information
        )
    ELSE()
        list(APPEND COMPILEOPT -std=c++20)
    ENDIF()
	
	target_compile_options(${PROJECT_NAME} PUBLIC  ${COMPILEOPT})	
	set_project_compile_options_debugrelease()
endfunction(set_project_compile_options)

function(set_project_definitions)
	#Unicode character set
	add_definitions(-DUNICODE -D_UNICODE)

	# Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger. 
	# The symbolic debugging information includes the names and types of variables, as well as functions and line numbers. 
	# /Zi does not affect optimizations. However, /Zi does imply /debug; see /DEBUG (Generate Debug Info) for more information. 
	IF( ${WIN32} ) 
        add_definitions(/Zi)
    ENDIF()

    IF( ${WIN32} ) 
        #Multi process compilation (currently default and cpu2 supported)
        IF(EXISTS "${RootDir}/Tools/CMake/cpu2")
            add_definitions(/MP2)  
        ELSEIF(EXISTS "${RootDir}/Tools/CMake/cpu3")
            add_definitions(/MP3)  
        ELSEIF(EXISTS "${RootDir}/Tools/CMake/cpu4")
            add_definitions(/MP4)  
        ELSEIF(EXISTS "${RootDir}/Tools/CMake/cpu5")
            add_definitions(/MP5)  
        ELSEIF(EXISTS "${RootDir}/Tools/CMake/cpu6")
            add_definitions(/MP6)  
        ELSEIF(EXISTS "${RootDir}/Tools/CMake/cpu7")
            add_definitions(/MP7)  
        ELSEIF(EXISTS "${RootDir}/Tools/CMake/cpu8")
            add_definitions(/MP8)  
        ELSE()
            add_definitions(/MP)
        ENDIF()
    ENDIF()
    
	#64bit define
	IF( "${CMAKE_SIZEOF_VOID_P}" STREQUAL "8" )
		IF( ${WIN32} ) 
			add_definitions(-D_WIN64)
		ENDIF()
		add_definitions(-D__x86_64__)
	ENDIF()
		
endfunction(set_project_definitions)

function(set_console_project_compile_flags)
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/INCREMENTAL /DEBUG:FASTLINK /SUBSYSTEM:CONSOLE /SAFESEH:NO /LARGEADDRESSAWARE")
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/INCREMENTAL:NO /DEBUG:FULL /SUBSYSTEM:CONSOLE /SAFESEH:NO /MAP /ignore:4099 /LARGEADDRESSAWARE")
endfunction(set_console_project_compile_flags)

function(set_project_processor_definitions)
    IF( ${WIN32} )	
        list (APPEND COMPILER_DEFINES
            _WINDOWS
        )
    ENDIF()

	list(APPEND COMPILER_DEFINES_DEBUG
		_DEBUG                    	
	)

	list(APPEND COMPILER_DEFINES_RELEASE
		NDEBUG                  
	)
	
	target_compile_definitions(${PROJECT_NAME} PUBLIC  ${COMPILER_DEFINES})
	target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:${COMPILER_DEFINES_DEBUG}>)
	target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Release>:${COMPILER_DEFINES_RELEASE}>)
endfunction(set_project_processor_definitions)

function(set_precompiled_header)
		set(FILENAME "stdafx.h")
		set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
        set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yu\"${FILENAME}\" /DUSE_PRECOMPILED_HEADERS")
        set_source_files_properties(${SOURCE} PROPERTIES COMPILE_FLAGS "/Yc")
endfunction(set_precompiled_header)

function(set_git_branch_in_label)
    IF (NOT WIN32) 
        return()
    ENDIF()

    find_package(Git)

	IF( !GIT_FOUND )
	   message(STATUS "Git executable was not found, check if you have Git installed.")
	   return()
	ENDIF()

	IF(NOT EXISTS "${RootDir}/.git")
		message(STATUS "Warning: .git folder does not exist.")
		return()
	ENDIF()

	execute_process(
		COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
		RESULT_VARIABLE result
		OUTPUT_VARIABLE output
	)
	
	STRING(REPLACE "\n" "" branch_name ${output})
	
	IF( NOT "${branch_name}" STREQUAL "master" )
		set_target_properties(${PROJECT_NAME} PROPERTIES 
								PROJECT_LABEL "${PROJECT_NAME}(${branch_name})")
	ENDIF()
endfunction(set_git_branch_in_label)

macro(set_project_console_mode)
	file(GLOB_RECURSE sources "*.cpp" "*.h" "*.c" "*.rc" "*.bmp" "*.cur" "*ico")
	set(root ${CMAKE_CURRENT_SOURCE_DIR})
	               
	list(REMOVE_ITEM sources "${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles/${CMAKE_VERSION}/CompilerIdCXX/CMakeCXXCompilerId.cpp")
	list(REMOVE_ITEM sources "${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles/${CMAKE_VERSION}/CompilerIdC/CMakeCCompilerId.c")
	source_group("" FILES ${sources})
	add_executable(${PROJECT_NAME} ${sources})
    include_root_dir()
	set_project_compile_options()
	set_project_definitions()
	set_console_project_compile_flags()
	set_project_processor_definitions()
	set_precompiled_header()
	set_git_branch_in_label()
	STRING (REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endmacro(set_project_console_mode)

function(set_project_bin_properties)
	set(RuntimeOutputNameDebug "${PROJECT_NAME}D")
	
	set_target_properties(${PROJECT_NAME} PROPERTIES 
						RUNTIME_OUTPUT_NAME_DEBUG ${RuntimeOutputNameDebug}
						RUNTIME_OUTPUT_DIRECTORY_DEBUG "${RootDir}/bin")
					  
	set_target_properties(${PROJECT_NAME} PROPERTIES 
						RUNTIME_OUTPUT_NAME_RELEASE ${PROJECT_NAME}
						RUNTIME_OUTPUT_DIRECTORY_RELEASE "${RootDir}/bin")
endfunction(set_project_bin_properties)

set_project_console_mode()
set_project_bin_properties()

#Entry point
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:\"wmainCRTStartup\"")