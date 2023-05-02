#########################
#		VARIABLES		#
#########################

# Folders and names
NAME			:=	ircserv	

HEADERS_DIR		:=	inc
HEADERS_FILES	:=	irc.hpp

HEADERS			:= $(addprefix $(HEADERS_DIR)/, $(HEADERS_FILES))

SRC_DIR			:=	src
SRC_FILES		:=	main.cpp

SRCS			:= $(addprefix $(SRC_DIR)/, $(SRC_FILES))

BUILD_DIR		:=	build
OBJS			:=	$(SRC_FILES:%.cpp=$(BUILD_DIR)/%.o)
DEPS			:=	$(SRC_FILES:%.cpp=$(BUILD_DIR)/%.d)
CCDEFS			:=	NAME=\"$(NAME)\"

# Compiler options
CC				:=	c++
DEBUG_FLAG		:=	-g3 #-fsanitize=address
CC_FLAGS		:=	-Wextra -Werror -Wall
CC_DEPS_FLAGS	:=	-MP -MMD
CC_DEFS_FLAGS	:=	$(foreach def,$(CCDEFS),-D $(def))

MAKE			:=	make -C

# define standard colors
_END			:=	\x1b[0m
_BOLD			:=	\x1b[1m
_UNDER			:=	\x1b[4m
_REV			:=	\x1b[7m
_GREY			:=	\x1b[30m
_RED			:=	\x1b[31m
_GREEN			:=	\x1b[32m
_YELLOW			:=	\x1b[33m
_BLUE			:=	\x1b[34m
_PURPLE			:=	\x1b[35m
_CYAN			:=	\x1b[36m
_WHITE			:=	\x1b[37m

#########################
# 		RULES			#
#########################

all: banner $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CC_FLAGS) $(OBJS) $(LIBS) -o $@ 

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile
	@mkdir -p $(@D)
	$(CC) $(CC_FLAGS) $(CC_DEPS_FLAGS) $(CC_DEFS_FLAGS) -I$(HEADERS_DIR) -c $< -o $@

clean: banner
	@rm -rf $(BUILD_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

banner:
	@echo ""	
	@echo " ____  ____       __  ____   ___ "
	@echo "(  __)(_  _)     (  )(  _ \ / __)"
	@echo " ) _)   )(  ____  )(  )   /( (__ "
	@echo "(__)   (__)(____)(__)(__\_) \___)"
	@echo ""	


.PHONY: all clean fclean re force banner
