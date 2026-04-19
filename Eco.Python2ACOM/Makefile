# Makefile for Eco.Python2ACOM
# Usage: make <command>
# List all commands: make help

.PHONY: help install format lint test test-cov style clean build pre-commit docs-serve docs-build all

# Variables
POETRY := poetry
PACKAGE := eco_python2acom

# ANSI color codes (works in modern terminals including Windows Terminal)
CYAN := \033[0;36m
GREEN := \033[0;32m
YELLOW := \033[0;33m
NC := \033[0m

help: ## Show this help message
	@printf "$(CYAN)Available commands:$(NC)\n"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  $(GREEN)%-15s$(NC) %s\n", $$1, $$2}'

install: ## Install dependencies via Poetry
	$(POETRY) install

format: ## Format code with Black and isort
	$(POETRY) run black .
	$(POETRY) run isort .

lint: ## Check code with Ruff
	$(POETRY) run ruff check .
	$(POETRY) run mypy .

test: ## Run tests
	$(POETRY) run pytest

test-cov: ## Run tests with coverage
	$(POETRY) run pytest --cov=$(PACKAGE) --cov-report=term-missing

style: format lint ## Run all style checks (format, lint)

pre-commit: ## Run pre-commit hooks on all files
	$(POETRY) run pre-commit run --all-files

docs-serve: ## Serve documentation locally
	$(POETRY) run mkdocs serve --open --clean

docs-build: ## Build documentation to site/
	$(POETRY) run mkdocs build --clean

clean: ## Clean cache files and temporary directories
	find . -type d -name "__pycache__" ! -path "./.venv/*" -exec rm -r {} + 2>/dev/null || true
	find . -type f -name "*.pyc" ! -path "./.venv/*" -delete 2>/dev/null || true
	find . -type f -name "*.pyo" ! -path "./.venv/*" -delete 2>/dev/null || true
	find . -type f -name "*.pyd" ! -path "./.venv/*" -delete 2>/dev/null || true
	find . -type d -name "*.egg-info" ! -path "./.venv/*" -exec rm -r {} + 2>/dev/null || true
	rm -rf .pytest_cache .mypy_cache .ruff_cache .coverage dist build .eggs 2>/dev/null || true

build: clean ## Build library for publishing
	$(POETRY) build

all: clean style build ## Clean, style and build (full cycle)
