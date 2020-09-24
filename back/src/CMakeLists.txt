option(ENGINE9_USE_CLOUD_AWS "Enable AWS" OFF)
option(ENGINE9_USE_CLOUD_GOOGLE "Enable Google" OFF)
option(ENGINE9_USE_CLOUD_AZURE "Enable Azure" OFF)

if(ENGINE9_USE_CLOUD_AWS OR ENGINE9_USE_CLOUD_GOOGLE OR ENGINE9_USE_CLOUD_AZURE)
	message("Cloud enabled")
	add_subdirectory(cloud)
endif()

if(ENGINE9_USE_CLOUD_AWS)
	message("Cloud... AWS enabled.")
	add_subdirectory(cloud/aws)
endif()

if(ENGINE9_USE_CLOUD_GOOGLE)
	message("Cloud... Google")
	add_subdirectory(cloud/google)
endif()

if(ENGINE9_USE_CLOUD_AZURE)
	message("Cloud... Azure")
	add_subdirectory(cloud/azure)
endif()

add_executable(engine9d main.cc)
target_link_libraries(
  engine9d
  PRIVATE project_options
          project_warnings
          CONAN_PKG::docopt.cpp
          CONAN_PKG::fmt
		  CONAN_PKG::spdlog
		  fcgi++
		  fcgi
		  )
