SRCS		:= main.cpp

OBJS		:= $(SRCS:.cpp=.o)
DEP			:= $(SRCS:.cpp=.d)
NAME		:= ircserv
CXX			:= clang++
CXXFLAGS	:= -std=c++98 -g3 -fstandalone-debug -MMD -MP #-Wall -Wextra -Werror

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
			$(RM) $(OBJS) $(DEP)
		
fclean:		clean
			$(RM) $(NAME)

re:			fclean all

-include $(DEP)