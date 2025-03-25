# Nome dell'eseguibile e cartelle
TARGET = bin/PrePy
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

# Setup Terminale
setupTerminal: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/PrePy
	echo "PrePy installato in /usr/local/bin/. Ora puoi eseguirlo da qualsiasi cartella digitando 'PrePy'."

# Build
build:
	make clean
	make

# Test
test:
	make clean
	make
	clear
	./bin/PrePy test.py path2

testDefaultPath:
	make clean
	make
	clear
	./bin/PrePy test.py

testConfig:
	make clean
	make
	clear
	./bin/PrePy config

# Regola per formattare il codice C++ con clang-format
format:
	@echo "Formattando i file C++..."
	@find $(SRC_DIR) -name "*.cpp" -exec clang-format -i {} \;
	@echo "Formattazione completata!"
