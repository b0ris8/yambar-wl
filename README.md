# yambar-wl

Панель для Wayland-композиторов (labwc, sway, river, niri…): форк [yambar](https://codeberg.org/dnkl/yambar),
только Wayland, без X11, D-Bus, systemd/elogind и xdg-desktop-portal.

Оригинал заброшен автором (последний коммит `abeffbd` «readme: goodbye!», май 2025), так что дальше он живёт здесь.
Его README сохранён как [README.upstream.md](README.upstream.md).

---

## Чем отличается от yambar

**Зависимости.** По умолчанию выключено всё, что тянет X11 или шину:

| Опция meson      | yambar   | yambar-wl | Почему                                                |
|------------------|----------|-----------|-------------------------------------------------------|
| `backend-x11`    | enabled  | disabled  | только Wayland                                        |
| `plugin-mpris`   | enabled  | disabled  | D-Bus через libsystemd / elogind / basu               |
| `plugin-xkb`     | auto     | disabled  | X11                                                   |
| `plugin-xwindow` | auto     | disabled  | X11                                                   |

К xdg-desktop-portal yambar не обращается вообще. Проверка после сборки:

```bash
ldd build/yambar | grep -Ei 'xcb|x11|dbus|systemd|elogind' || echo чисто
```

**Пилюли.** У декорации `background` новые параметры — скругление, отступ от краёв панели, рамка и мягкая тень:

| Параметр        | Что делает                                                        | По умолчанию |
|-----------------|-------------------------------------------------------------------|--------------|
| `radius`        | скругление углов, px (больше половины высоты — режется)           | 0            |
| `margin`        | отступ фона от верха и низа панели, px                            | 0            |
| `border-width`  | толщина рамки внутри формы, px; нужен `border-color`              | 0            |
| `border-color`  | цвет рамки, `RRGGBBAA`                                            | —            |
| `shadow-size`   | длина размытия тени вокруг формы, px; нужен `shadow-color`        | 0            |
| `shadow-color`  | цвет тени у края формы, дальше плавно до прозрачного              | —            |
| `shadow-offset` | сдвиг тени вниз, px                                               | 1            |

У `underline` — `inset`: укоротить линию на N px с обеих сторон.

Панель разрешает декорациям заходить в свои боковые поля (`bar.margin`), чтобы тень крайних пилюль не обрезалась.
Текст и значки туда как не попадали, так и не попадают.

Всё описано в `man yambar-decorations` после установки или в [doc/yambar-decorations.5.scd](doc/yambar-decorations.5.scd).

---

## Сборка и установка

Нужны пакеты (Arch / Artix):

```bash
sudo pacman -S --needed meson ninja scdoc tllist fcft pixman libyaml wayland wayland-protocols flex bison
# по желанию, для модулей: alsa-lib libmpdclient libpulse pipewire json-c
```

Сборка и установка в `~/.local/bin`, без root:

```bash
git clone https://github.com/b0ris8/yambar-wl.git
cd yambar-wl
meson setup build --buildtype=release --wrap-mode=nofallback
ninja -C build
install -m755 build/yambar ~/.local/bin/
```

Бинарник называется `yambar`, конфиг — `~/.config/yambar/config.yml`, как у оригинала.
Если в системе остался пакетный yambar, запускайте по полному пути `~/.local/bin/yambar`
или удалите пакет: `sudo pacman -R yambar`.

Обновить после изменений в коде:

```bash
git pull
ninja -C build && install -m755 build/yambar ~/.local/bin/
```

Если папку репозитория переносили — сначала `rm -rf build` и заново `meson setup`: build хранит абсолютные пути.

---

## Конфиг

Готовый пример в стиле пилюль (Nord + тёмные `#232731` / `#16191f`): [examples/configurations/pill.yml](examples/configurations/pill.yml).

```bash
cp examples/configurations/pill.yml ~/.config/yambar/config.yml
~/.local/bin/yambar --validate
```

Основа — фон с рамкой и тенью вешается на весь `list`, а не на каждую строку, тогда пилюля одна на модуль:

```yaml
pill_bg: &pill_bg 232731ff
pill_shadow: &pill_shadow 16191fff
pill_cyan: &pill_cyan
  background: {color: *pill_bg, radius: 5, margin: 4, border-width: 1, border-color: 88c0d080,
               shadow-size: 8, shadow-color: *pill_shadow}

bar:
  height: 28        # пилюля = height - 2*margin
  margin: 8         # боковые поля под тень крайних пилюль (>= shadow-size)
  spacing: 4        # промежуток между пилюлями = 2*spacing
  ...
  right:
    - cpu:
        content:
          map:
            conditions:
              id == -1:
                list:
                  left-margin: 8
                  right-margin: 8
                  deco: *pill_cyan
                  items:
                    - string: {text: "{cpu}%"}
```

Что крутить:

| Хочу                              | Где                                                     |
|-----------------------------------|---------------------------------------------------------|
| скругление больше / меньше        | `radius`; 0 — прямые углы, половина высоты — капсула     |
| пилюля ниже / выше                | `bar.height` и `margin`                                 |
| рамка ярче / тише                 | альфа в `border-color`: `...80` — 50%, `...cc` — 80%    |
| тень длиннее / короче             | `shadow-size`; `bar.margin` не меньше                    |
| тень плотнее / светлее            | цвет `shadow-color`                                     |
| отступ текста внутри пилюли       | `left-margin` / `right-margin` у `list`                 |

Стоковый yambar на этих ключах откажется стартовать (`invalid key: radius`) — значит, запустился не тот бинарник.

---

## Запуск

В `~/.config/labwc/autostart`:

```sh
"$HOME/.local/bin/yambar" &
```

Перезапуск из любого места с dbus-окружением сессии labwc — скрипты `yambar-restart` и `restart-desktop-apps`
(в репозитории `infra`); в них тоже полный путь `$HOME/.local/bin/yambar`.

---

## Происхождение и лицензия

Форк [dnkl/yambar](https://codeberg.org/dnkl/yambar) с коммита `abeffbd`, удалёнка `upstream`.
Лицензия MIT, как у оригинала — [LICENSE](LICENSE).
