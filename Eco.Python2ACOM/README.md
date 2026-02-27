# Eco.Python2ACOM

Библиотека для работы с ACOM в Python.

## Установка

```bash
poetry add eco-python2acom
```

## Разработка

### Использование Makefile

Проект включает `Makefile` для удобной работы с инструментами разработки.

```bash
make help          # Показать все доступные команды
make install       # Установить зависимости
make format        # Форматировать код
make lint          # Проверить код линтером
make lint-fix      # Автоисправить ошибки линтера
make typecheck     # Проверить типы
make test          # Запустить тесты
make test-cov      # Запустить тесты с покрытием
make check         # Запустить все проверки
make pre-commit    # Запустить pre-commit hooks на всех файлах
make clean         # Очистить кэш-файлы
make build         # Собрать библиотеку
make all           # Полный цикл: очистка, проверка, сборка
```
