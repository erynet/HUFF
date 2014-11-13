@echo on
ECHO [COMPRESS] pg1342.txt -> pg1342.txt.huff
PAUSE
@echo off
encode_binary_file.exe pg1342.txt
@echo on
ECHO [DECOMPRESS] pg1342.txt.huff -> dec_pg1342.txt
PAUSE
@echo off
decode_binary_file.exe pg1342.txt.huff dec_pg1342.txt
PAUSE

@echo on
ECHO [COMPRESS] pg28233.txt -> pg28233.txt.huff
PAUSE
@echo off
encode_binary_file.exe pg28233.txt
@echo on
ECHO [DECOMPRESS] pg28233.txt.huff -> dec_pg28233.txt
PAUSE
@echo off
decode_binary_file.exe pg28233.txt.huff dec_pg28233.txt
PAUSE

@echo on
ECHO [COMPRESS] pg38425.jpg -> pg38425.jpg.huff
PAUSE
@echo off
encode_binary_file.exe pg38425.jpg
@echo on
ECHO [DECOMPRESS] pg38425.jpg.huff -> pg38425.jpg.txt
PAUSE
@echo off
decode_binary_file.exe pg38425.jpg.huff dec_pg38425.jpg
PAUSE