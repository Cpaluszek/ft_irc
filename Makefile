#########################
#		VARIABLES		#
#########################

# Folders and names
NAME			:=	ircserv	

HEADERS_DIR		:=	inc
HEADERS_FILES	:=

HEADERS			:= $(addprefix $(HEADERS_DIR)/, $(HEADERS_FILES))

SRC_DIR			:=	src
SRC_FILES		:=

SRCS			:= $(addprefix $(SRC_DIR)/, $(SRC_FILES))

BUILD_DIR		:=	build
OBJS			:=	$(SRC_FILES:%.cpp=$(BUILD_DIR)/%.o)

# Compiler options
CC				:=	c++
DEBUG_FLAG		:=	-g3 #-fsanitize=address
CC_FLAGS		:=	-Wextra -Werror -Wall

MAKE			:=	make -C

#########################
# 		RULES			#
#########################

all:
	$(CC) $(CC_FLAGS) socket_example/server.cpp -o serv
	$(CC) $(CC_FLAGS) socket_example/client.cpp -o client

clean:
	@rm serv client

fclean: clean

re: fclean all

.PHONY: all clean fclean re
