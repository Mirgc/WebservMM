NAME 		= webserv
NAME_SAN 	= webserv_sanitized

CC		= c++

DEBUG		= -fsanitize=address -g3 -D DEBUG
CFLAGS 		= -Wall -Werror -Wextra -std=c++98 --pedantic -MD
RM 		= rm -rf

SRCS_MAIN = ./
SRCS_DIR  = src/
OBJS_DIR  = obj/
INCS_DIR  = inc/


HEADERS	 = ./$(INCS_DIR)
INCLUDES = $(addprefix -I, $(HEADERS))


SRCS := $(wildcard $(SRCS_MAIN)main.cpp) $(wildcard $(SRCS_DIR)*.cpp)

OBJS := $(addprefix $(OBJS_DIR), $(notdir $(SRCS:.cpp=.o)))

all: ccinfo $(NAME)

$(NAME): $(OBJS)
		@$(CC) $(CFLAGS) $^ -o $@ 
		@echo "\n\033[92m"-------------\\n👌 COMPILED 👌\\n-------------\\n"\033[0m\n"

$(OBJS_DIR)%.o: $(SRCS_MAIN)%.cpp 
		@mkdir -p $(OBJS_DIR)
		@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp 
		@mkdir -p $(OBJS_DIR)
		@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

san: $(NAME_SAN)

$(NAME_SAN): $(OBJS)
		@$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ 
		@echo "\n\033[92m"--------------\\n👌 SANITIZED 👌\\n--------------\\n"\033[0m\n"

clean:
		@$(RM) $(OBJS_DIR)
		@$(RM) *.dSYM
		@echo "\n\033[31m"-------------------\\n💣 DELETED FILES 💣\\n-------------------\\n"\033[0m\n"

fclean: clean
		@$(RM) $(NAME)
		@$(RM) $(NAME_SAN)
		@echo "\n\033[31m"-------------------\\n💣 FULL CLEAN 💣\\n-------------------\\n"\033[0m\n"

re: fclean all

ccinfo: 
		@$(CC) -v

-include $(OBJS_DIR)/*.d

.PHONY: all clean fclean ccinfo %.o
