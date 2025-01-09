# Project MISP: "Берсерк"

**Project MISP** — это двухмерная компьютерная игра, вдохновленная мангой и аниме "Берсерк". Игроку предстоит управлять персонажем, сражаться с волнами врагов на различных уровнях и достигать финальной цели — бесконечных битв на последнем этапе. Игра разработана с использованием библиотеки SFML и языка программирования C++.

## Требования

Для сборки и запуска игры необходимы следующие компоненты:

- **Visual Studio 2022**
- **MSVC (Microsoft Visual C++) v143**
- **C++17**
- **SFML 2.6.1** — убедитесь, что у вас установлена соответствующая версия библиотеки.

## Основные механики игры

- Персонаж с уникальными характеристиками (здоровье, выносливость, скорость, урон).
- Враги с разными параметрами и стилями поведения.
- Система уровней, представляющих различные локации:
  - Замок Долдрей.
  - Темный лес.
  - Пространство Вечности.
- Возможность сохранения и загрузки прогресса.
- Меню настроек с изменением характеристик персонажа.
- Поддержка анимаций и музыкального сопровождения.

## Руководство по сборке и запуску

### 1. Установка SFML
1. Скачайте библиотеку SFML версии 2.6.1 с [официального сайта](https://www.sfml-dev.org/).
2. Переместите скачанную библиотеку в директорию:  
   `D:\libraries\SFML-2.6.1`.

### 2. Клонирование репозитория
1. Склонируйте репозиторий игры с GitHub, используя следующую команду:
   ```bash
   git clone https://github.com/AVeukov/Project_MISP