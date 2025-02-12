NAME = stack
SOURCES = $(wildcard *.cpp)
SRC = $(patsubst %.cpp, %.o, $(SOURCES))
all: $(NAME)
$(NAME): $(SRC)
	@g++ $(SRC) -o $@.exe
%.o : %.cpp 
	@g++ -c $< -o $@