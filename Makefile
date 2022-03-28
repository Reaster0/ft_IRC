SRCS		:= main.cpp \
				ServerClass.cpp \
				socket.cpp \
				debug.cpp \
				MsgIRC.cpp \
				UserIRC.cpp \
				Channel.cpp \
				handlerFunction.cpp \

OBJS_PATH 	= objs
DEPS    	= $(patsubst %, $(OBJS_PATH)/%, $(SRCS:.cpp=.d))
OBJS   		= $(patsubst %, $(OBJS_PATH)/%, $(SRCS:.cpp=.o))
NAME		:= ircserv

CXX			:= clang++
CXXFLAGS	:= -std=c++98 -g3 -fstandalone-debug -fsanitize=address -MMD -MP -g -Wall -Wextra -Werror
RM       	= rm -rf

all:		$(NAME) $(NAME_2)

$(OBJS_PATH)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME):	$(OBJS_PATH) $(OBJS)
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)


$(OBJS_PATH):
	mkdir -p $(OBJS_PATH)

clean:
			$(RM) $(OBJS_PATH)
		
fclean:		clean
			$(RM) $(NAME) 

re:			fclean all

-include $(DEPS)
