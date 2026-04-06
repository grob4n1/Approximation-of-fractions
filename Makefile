all: rebuild

rebuild: clean build

.ONESHELL:
build:
	python -m venv graph_venv
	.\graph_venv\Scripts\activate && pip install matplotlib && python .\create_graph.py && deactivate


clean:
	rmdir /s /q ".\graph_venv"
