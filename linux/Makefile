PRODUCT_NAME=MindKeyServer
BOOST_ROOT=/usr/local/boost_1_55_0
BOOST_INCLUDE_DIR=$(BOOST_ROOT)
BOOST_LIB_DIR=/usr/lib
BOOST_LINK_FLAGS=-lboost_serialization -lboost_system 
MindKeyServer: ../src/MindKeyServer/mindKeyServer.cpp
	g++ ../src/MindKeyServer/mindKeyServer.cpp -I../src/shared/ -I$(BOOST_INCLUDE_DIR) -L$(BOOST_LIB_DIR) $(BOOST_LINK_FLAGS) -o $(PRODUCT_NAME)