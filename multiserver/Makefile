NAME 		= webserv
SRC_DIR		= ./src
SRC 		= 	$(SRC_DIR)/main.cpp \
				$(SRC_DIR)/VirtualHostServer.cpp \
				$(SRC_DIR)/Reactor.cpp \
				$(SRC_DIR)/EventHandler.cpp

CC		= c++
DEBUG		= -fsanitize=address -g3 -D DEBUG
# CFLAGS 		= -Wall -Werror -Wextra -std=c++98 $(DEBUG)
CFLAGS 		= -std=c++98 $(DEBUG)
OBJECTS		= $(SRC:.cpp=.o)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $^

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)
	@echo "\n\033[92m"-------------\\n👌 COMPILED 👌\\n-------------\\n"\033[0m\n"

all: $(NAME)

clean:
	@echo "\n\033[31m"-------------------\\n💣 DELETED FILES 💣\\n-------------------\\n"\033[0m\n"
	@rm -f $(OBJECTS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean re test
