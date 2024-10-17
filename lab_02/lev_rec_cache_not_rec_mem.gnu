set style data lines

set title "График сравнения по памяти линейного и рекурсивного с\nкешированием алгоритмов поиска расстояния Левенштейна"
set xlabel "N"
set ylabel "T"

set xrange [1:*]
set yrange [0:*]

set key below

set style line 1 lt 1 lw 2 lc rgb "red"   # Сплошная линия
set style line 2 lt 2 lw 2 lc rgb "blue"  # Пунктирная линия

plot "data/all_mesurements_data.txt" using 1:2 title "Линейная функция поиска расстояния Левенштейна" with lines linestyle 1, \
     "" using 1:4 title "Рекурсивная функция поиска расстояния Левенштейна с кешированием" with lines linestyle 2

set terminal svg size 620,480 enhanced
set output 'docs/inc/img/lev_rec_cache_not_rec_mem.svg'
replot
set output
