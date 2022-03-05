SRCS		:= main.cpp \
				ClientList.cpp
OBJS		:= $(SRCS:.cpp=.o)
DEP			:= $(SRCS:.cpp=.d)
NAME		:= ircserv

SRCS_2		:= client_tester.cpp
OBJS_2		:= $(SRCS_2:.cpp=.o)
DEP_2		:= $(SRCS_2:.cpp=.d)
NAME_2		:= client

CXX			:= clang++
CXXFLAGS	:= -std=c++98 -g3 -fstandalone-debug -MMD -MP #-Wall -Wextra -Werror

all:		$(NAME) $(NAME_2)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(NAME_2): $(OBJS_2)
			$(CXX) $(CXXFLAGS) $(OBJS_2) -o $(NAME_2)

clean:
			$(RM) $(OBJS) $(DEP)
			$(RM) $(OBJS_2) $(DEP_2)
		
fclean:		clean
			$(RM) $(NAME) 
			$(RM) $(NAME_2)

re:			fclean all

-include $(DEP)