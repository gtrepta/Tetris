LDFLAGS = -lSDL2 -lGL -lGLU
OBJDIR = objs
SRCDIR = Tetris
BINDIR = bin
DEPS = $(wildcard $(SRCDIR)/*.h)
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
CC = g++

Tetris : $(OBJS)
	$(CC) -o $(BINDIR)/$@ $^ $(LDFLAGS)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/Tetris
