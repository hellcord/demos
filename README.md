# Демки HellCord

Живой код из шортсов сервера [HellCord](https://discord.gg/hellcord). Шортсы — на YouTube:
[@hellcord-server](https://www.youtube.com/@hellcord-server). Каждая демка — одна папка со статической
страницей, без сборки и зависимостей. Сайт: https://hellcord.github.io/demos/

| Демка | Что | Шортс |
| --- | --- | --- |
| [shader-flower](shader-flower/) | Неоновый цветок: 8 строк фрагментного шейдера (WebGL 1, GLSL ES 1.0), код правится прямо на странице | «Это не видео. Это 8 строк кода» |

## Неоновый цветок

```glsl
precision highp float;
uniform vec2 R;   // размер холста в пикселях
uniform float T;  // время в секундах

void main() {
  vec2 p = (2.*gl_FragCoord.xy - R) / R.y;      // координаты пикселя, центр — 0
  float r = length(p);                          // расстояние до центра
  float a = atan(p.y, p.x);                     // угол
  float f = r + .15*sin(6.*a + T);              // синус от угла — 6 лепестков
  float g = .02 / abs(fract(4.*f - .5*T) - .5); // fract — кольца, деление — свечение
  vec3 k = vec3(0, .1, .2);
  vec3 c = .5 + .5*cos(6.3*(f + .1*T + k));     // косинусная палитра
  gl_FragColor = vec4(c * g * exp(-r), 1);
}
```

Рисуется двумя треугольниками на весь экран — вся картинка во фрагментном шейдере.

## Новая демка

Папка `имя/` с `index.html` (всё внутри файла), строка в таблице выше и карточка в корневом `index.html`.
