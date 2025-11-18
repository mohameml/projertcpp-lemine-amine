# -----------------------------------------------------------
#   COMPILER SETTINGS
# -----------------------------------------------------------

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic

# -----------------------------------------------------------
#   TARGET & DIRECTORIES
# -----------------------------------------------------------

BINDIR = bin
TARGET = pricing_test
BIN_TARGET = $(BINDIR)/$(TARGET)

SRC = main.cpp \
      BSModel.cpp \
      Option.cpp \
      PricingMC.cpp

# Tous les .o se trouveront dans bin/
OBJ = $(patsubst %.cpp,$(BINDIR)/%.o,$(SRC))

# -----------------------------------------------------------
#   RULES
# -----------------------------------------------------------

all: $(BIN_TARGET)

# Création du dossier bin si nécessaire
$(BINDIR):
	mkdir -p $(BINDIR)

# Link final dans bin/
$(BIN_TARGET): $(OBJ) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation des .cpp -> bin/xxx.o
$(BINDIR)/%.o: %.cpp | $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# -----------------------------------------------------------
#   CLEAN
# -----------------------------------------------------------

clean:
	rm -f $(BINDIR)/*.o

fclean: clean
	rm -rf $(BINDIR)

re: fclean all

.PHONY: all clean fclean re
