#########################
#		VARIABLES		#
#########################
NAME			:=	ircserv

HEADERS_DIR		:=	inc
HEADERS_FILES	:=	Server.hpp Client.hpp Utils.hpp Request.hpp \
					Channel.hpp \
					numericReplies.hpp commands.hpp colors.hpp flags.hpp

HEADERS			:= $(addprefix $(HEADERS_DIR)/, $(HEADERS_FILES))

SRC_DIR			:=	src
SRC_FILES		:=	main.cpp Server.cpp Client.cpp Utils.cpp Request.cpp \
					Channel.cpp \
					cmds/pass.cpp \
					cmds/nick.cpp \
					cmds/user.cpp \
					cmds/quit.cpp \
					cmds/privmsg.cpp \
					cmds/join.cpp \
					cmds/list.cpp \
					cmds/who.cpp \
					cmds/part.cpp \
					cmds/ping.cpp \
					cmds/topic.cpp \
					cmds/whois.cpp \
					cmds/motd.cpp \
					cmds/names.cpp \
					cmds/away.cpp \
					cmds/invite.cpp \
					cmds/mode.cpp \
					cmds/kick.cpp

SRCS			:= $(addprefix $(SRC_DIR)/, $(SRC_FILES))

BUILD_DIR		:=	build
OBJS			:=	$(SRC_FILES:%.cpp=$(BUILD_DIR)/%.o)

CC				:=	c++
DEBUG_FLAG		:=	-g3 -D DEBUG
CC_FLAGS		:=	-Wextra -Werror -Wall -std=c++98 $(DEBUG_FLAG)

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

debug: all
	valgrind --show-leak-kinds=all --leak-check=full --track-fds=yes --verbose --show-mismatched-frees=yes --read-var-info=yes ./$(NAME) 6697 test

$(NAME): $(OBJS)
	$(CC) $(CC_FLAGS) $(OBJS) $(LIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -I$(HEADERS_DIR) -c $< -o $@

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

.PHONY: all clean fclean re banner debug
