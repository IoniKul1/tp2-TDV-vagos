# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

# Nombre del ejecutable
TARGET = my_vrp_solver

# Archivos fuente
SRC = \
    main.cpp \
    Clases/VRPLIBReader.cpp \
    Heuristicas/CW.cpp \
    Heuristicas/InsertionSolver.cpp \
    Operadores_busqueda_local/relocate.cpp \
    Operadores_busqueda_local/swap.cpp \
    Operadores_busqueda_local/two_opt.cpp \
    Testings/testings.cpp

# Regla principal
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
