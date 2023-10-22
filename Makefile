NAME 		= webserv
NAME_V 		= webserv_sanitized

CC		= c++

DEBUG		= -fsanitize=address -g3 -D DEBUG
CFLAGS 		= -Wall -Werror -Wextra -std=c++98 --pedantic -MD $(DEBUG)
RM 		= rm -rf

SRCS_MAIN = ./
SRCS_DIR  = src/
OBJS_DIR  = obj/
INCS_DIR  = inc/


HEADERS	 = ./$(INCS_DIR)
INCLUDES = $(addprefix -I, $(HEADERS))


SRCS := $(wildcard $(SRCS_MAIN)main.cpp) $(wildcard $(SRCS_DIR)*.cpp)
SRCS_V := $(wildcard $(SRCS_MAIN)main_v.cpp) $(wildcard $(SRCS_DIR)*_v.cpp)

OBJS := $(addprefix $(OBJS_DIR), $(notdir $(SRCS:.cpp=.o)))
OBJS_V := $(addprefix $(OBJS_DIR), $(notdir $(SRCS_V:.cpp=.o)))

$(NAME): $(OBJS)
		@$(CC) $(CFLAGS) $^ -o $@ 
		@echo "\n\033[92m"-------------\\n👌 COMPILED 👌\\n-------------\\n"\033[0m\n"

$(OBJS_DIR)%.o: $(SRCS_MAIN)%.cpp 
		@mkdir -p $(OBJS_DIR)
		@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp 
		@mkdir -p $(OBJS_DIR)
		@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

san : $(NAME_V)

$(NAME_V): $(OBJS)
		@$(CC) $(CFLAGS) $(S) $^ -o $@ 
		@echo "\n\033[92m"-------------\\n👌 SANITIZEZ 👌\\n-------------\\n"\033[0m\n"

all: $(NAME)

clean:
		@$(RM) $(OBJS_DIR)
		@$(RM) *.dSYM
		@echo "\n\033[31m"-------------------\\n💣 DELETED FILES 💣\\n-------------------\\n"\033[0m\n"

fclean: clean
		@$(RM) $(NAME)
		@$(RM) $(NAME_V)
		@echo "\n\033[31m"-------------------\\n💣 FULL CLEAN 💣\\n-------------------\\n"\033[0m\n"

re: fclean all

-include $(OBJS_DIR)/*.d

.PHONY: all clean fclean %.o
