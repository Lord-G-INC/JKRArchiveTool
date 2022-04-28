Include_Dir := Include

CPPFILES := $(foreach dir,Source,$(wildcard $(dir)/*.cpp))

ifeq ($(OS),Windows_NT)
	TARGET := JKRArchiveTools.exe
else
	TARGET := JKRArchiveTools
endif

all: $(TARGET)

$(TARGET): $(CPPFILES)
	g++ -s -Os -I $(Include_Dir) $^ -o $(TARGET)

clean:
	rm $(TARGET)