@echo off
cd ..\lib\protobuf
protoc --cpp_out=. hid_msg.proto
cd ..\..\script
echo ok
