@echo off
cd ..\lib\protobuf
protoc --cpp_out=. usb_comm.proto
cd ..\..\script
echo ok
