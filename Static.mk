CPPFILES := Source\BinaryReaderAndWriter.cpp Source\JKRArchive.cpp Source\Util.cpp

TARGET := JKRArchiveTool.a

all: $(TARGET)

$(TARGET):
	g++ -I Include $(CPPFILES) -c
	ar rcs $@ *.o
	rm *.o