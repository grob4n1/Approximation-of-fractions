all: build compile run

run: 
	.\Fraction.exe

compile: .\Fraction.cpp
	g++ .\Fraction.cpp -o .\Fraction.exe

rebuild: clean build

.ONESHELL:
build: graph_venv
	.\graph_venv\Scripts\activate && python .\create_graph.py && deactivate

graph_venv:
	python -m venv graph_venv
	.\graph_venv\Scripts\activate && pip install matplotlib && deactivate

clean:
	rmdir /s /q ".\graph_venv"




