# Поиск точки пересечения двух отрезков в пространстве

## Дано:
Два отрезка Segment3D, каждый из которых имеет начало и конец, которые являются точками Vector3D в пространстве.

## Результат:
Точка пересечения двух отрезков, либо невалидная точка, если точек пересечения нет.

## Метод решения:
Проверить, является ли отрезок точкой (начало и конец совпадают), есть ли пересечение ограничивающих параллелепипедов (AABB), лежат ли отрезки в одной плоскости (не скрещиваются), параллельны ли отрезки, совпадают ли они. 
Если пройдены все эти проверки, то прямые, на кооторых лежат отрезки, пересекаются в одной отчке.
Для нахождения точки пересечения прямых решается СЛАУ (в проекции на координатную плоскость UV находятся параметры для уравнения прямых), затем проверяется, принадлежит ли данная точка отрезкам (параметры уравнения прямых лежат в промежутке от 0 до 1).

Возможны случаи, когда один из отрезоков направлен параллельно координатной оси. В таких случаях с высокой вероятностью возникает ошибка деления на 0.
Поэтому перед решением СЛАУ, мы определяем координатную плоскость UV, в которой такой ошибки не возникнет.

## Тесты программы:
### 1. Оба отрезка - точки, совпадают

Segment3D seg1({ 1, 1, 1 }, { 1, 1, 1 });<br>
Segment3D seg2({ 1, 1, 1 }, { 1, 1, 1 });<br>
<br>
Segments intersection:<br>
Segment 1 is point<br>
Segment-point intersection:<br>
Intersection: 1 1 1<br>
<br>

### 2. Один из отрезков - точка, лежит на втором

Segment3D seg1({ 0, 0, 0 }, { 2, 2, 2 });<br>
Segment3D seg2({ 1, 1, 1 }, { 1, 1, 1 });<br>
<br>
Segments intersection:<br>
Segment 2 is point<br>
Segment-point intersection:<br>
Intersection: 1 1 1<br>
<br>

### 3. Один из отрезков - точка, не лежит на втором

Segment3D seg1({ 0, 0, 0 }, { 2, 2, 2 });<br>
Segment3D seg2({ 1.3, 1.2, 1.1 }, { 1.3, 1.2, 1.1 });<br>
<br>
Segments intersection:<br>
Segment 2 is point<br>
Segment-point intersection:<br>
Point is not on the line<br>
Intersection: nan nan nan<br>
<br>

### 4. Один из отрезков - точка, не в ограничивающем параллелепипеде второго

Segment3D seg1({ 0, 0, 0 }, { 2, 2, 2 });<br>
Segment3D seg2({ 11.3, 1.2, 1.1 }, { 11.3, 1.2, 1.1 });<br>
<br>
Segments intersection:<br>
Segment 2 is point<br>
Segment-point intersection:<br>
Point is outside the bounding box<br>
Intersection: nan nan nan<br>
<br>

### 5. Два отрезка, ограничивающие параллелепипеды не пересекаются

Segment3D seg1({ 0, 0, 0 }, { 2, 2, 2 });<br>
Segment3D seg2({ -1, -1, -1 }, { -2, -2, -2 });<br>
<br>
Segments intersection:<br>
Bounding boxes do not overlap<br>
Intersection: nan nan nan<br>
<br>

### 6. Два отрезка параллельны, не накладываются друг на друга

Segment3D seg1({ 0, 0, 0 }, { 2, 2, 2 });<br>
Segment3D seg2({ 1, 0, 0 }, { 3, 2, 2 });<br>
<br>
Segments intersection:<br>
Parallel lines<br>
Intersection: nan nan nan<br>
<br>

### 7. Два отрезка параллельны, накладываются друг на друга

Segment3D seg1({ 0, 0, 0 }, { 2, 2, 2 });<br>
Segment3D seg2({ 1, 1, 1 }, { 3, 3, 3 });<br>
<br>
Segments intersection:<br>
Same line<br>
Intersection: 1 1 1<br>
<br>

### 8. Два отрезка совпадают

Segment3D seg1({ 0, 0, 0 }, { 2, 2, 2 });<br>
Segment3D seg2({ 0, 0, 0 }, { 2, 2, 2 });<br>
<br>
Segments intersection:<br>
Same line<br>
Intersection: 0 0 0<br>
<br>

### 9. Скрещивающиеся отрезки

Segment3D seg1({ 0, 0, 0 }, { 2, 2, 2 });<br>
Segment3D seg2({ 0, 2, 1 }, { 2, 0, 3 });<br>
<br>
Segments intersection:<br>
Skew lines<br>
Intersection: nan nan nan<br>
<br>

### 10. Пересекающиеся отрезки

Segment3D seg1({ 0, 0, 0 }, { 2, 2, 2 });<br>
Segment3D seg2({ 0, 2, 0 }, { 2, 0, 2 });<br>
<br>
Segments intersection:<br>
U: X<br>
V: Y<br>
t: 0.5 k: 0.5<br>
Intersection: 1 1 1<br>
<br>

### 11. Оба отрезка лежат в плоскости XY

Segment3D seg1({ 0, 0, 0 }, { 2, 2, 0 });<br>
Segment3D seg2({ 0, 2, 0 }, { 2, 0, 0 });<br>
<br>
Segments intersection:<br>
U: X<br>
V: Y<br>
t: 0.5 k: 0.5<br>
Intersection: 1 1 0<br>
<br>

### 12. Оба отрезка лежат в плоскости XZ

Segment3D seg1({ 0, 0, 0 }, { 2, 0, 2 });<br>
Segment3D seg2({ 0, 0, 2 }, { 2, 0, 0 });<br>
<br>
Segments intersection:<br>
U: X<br>
V: Z<br>
t: 0.5 k: 0.5<br>
Intersection: 1 0 1<br>
<br>

### 13. Оба отрезка лежат в плоскости YZ

Segment3D seg1({ 0, 0, 0 }, { 0, 2, 2 });<br>
Segment3D seg2({ 0, 0, 2 }, { 0, 2, 0 });<br>
<br>
Segments intersection:<br>
U: Y<br>
V: Z<br>
t: 0.5 k: 0.5<br>
Intersection: 0 1 1<br>
<br>

### Также было измерено среднее время работы функции
1000 повторении на случайной выборке с процентом пересечений 4.5%, один поток<br>
<br>
Time ns: 35<br>
Intersections: 45<br>
<br>

Процессор: 11th Gen Intel(R) Core(TM) i7-11800H @ 2.30GHz
Компилятор: MSVC, Стандарт: C++14, тесты проводились в конфигурации Release x64
