set style data lines

set title "График сравнения по времени рекурсивных алгоритмов\nпоиска расстояния Левенштейна с кешированием и без"
set xlabel "N"
set ylabel "T"

set xrange [1:9]
set yrange [0:*]

set key below

set style line 1 lt 1 lw 2 lc rgb "red"   # Сплошная линия
set style line 2 lt 2 lw 2 lc rgb "blue"  # Пунктирная линия

plot "data/all_mesurements.txt" using 1:3 title "Без кеширования" with lines linestyle 1, \
     "data/all_mesurements.txt" using 1:4 title "С кешированием" with lines linestyle 2

set terminal svg size 620,480 enhanced
set output 'docs/inc/img/rec_cmp.svg'
replot
set output
