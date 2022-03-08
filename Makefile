SRCS		:= main.cpp \
				random_password.cpp \
				socket.cpp \
				debug.cpp \
				MsgIRC.cpp \
				UserIRC.cpp \
				Channel.cpp \
				ServerClass.cpp \

OBJS_PATH 	= objs
DEPS    	= $(patsubst %, $(OBJS_PATH)/%, $(SRCS:.cpp=.d))
OBJS   		= $(patsubst %, $(OBJS_PATH)/%, $(SRCS:.cpp=.o))
NAME		:= ircserv

SRCS_2		:= client_tester.cpp
OBJS_2		:= $(patsubst %, $(OBJS_PATH)/%, $(SRCS_2:.cpp=.o))
DEP_2		:= $(patsubst %, $(OBJS_PATH)/%, $(SRCS_2:.cpp=.d))
NAME_2		:= client

CXX			:= clang++
CXXFLAGS	:= -std=c++98 -g3 -fstandalone-debug -MMD -MP #-Wall -Wextra -Werror
RM       	= rm -rf

all:		$(NAME) $(NAME_2)

$(OBJS_PATH)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(NAME):	$(OBJS_PATH) $(OBJS)
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(NAME_2): $(OBJS_PATH) $(OBJS_2)
			$(CXX) $(CXXFLAGS) $(OBJS_2) -o $(NAME_2)

$(OBJS_PATH):
	mkdir -p $(OBJS_PATH)

clean:
			$(RM) $(OBJS_PATH)
		
fclean:		clean
			$(RM) $(NAME) 
			$(RM) $(NAME_2)

re:			fclean all

-include $(DEP)
