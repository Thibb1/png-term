TARGET = pngterm
all:
	$(CXX) PngTerm.cpp PngTerm.hpp -lncursesw -lpng -o $(TARGET) -std=c++2a
clean:
	$(RM) -f $(TARGET)