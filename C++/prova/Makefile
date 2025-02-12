# Nome dell'eseguibile e cartelle
TARGET = bin/Main
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Compilatore e opzioni
CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17 -I$(INC_DIR)

# Trova tutti i file .cpp in src/ e crea i corrispondenti file .o in obj/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Regola principale: compila tutto
$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Regola per compilare ogni .cpp in .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crea la cartella obj/ se non esiste
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Pulizia
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

run:
	rm -rf $(OBJ_DIR) $(TARGET)
	make
	./bin/Main