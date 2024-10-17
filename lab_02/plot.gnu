set style data lines

set title "График сравнения по времени алгоримов умножения матриц"
set xlabel "N"
set ylabel "T"

set xrange [1:*]
set yrange [0:*]

set key below

set style line 1 lt 1 lw 2 lc rgb "red"   # Сплошная линия
set style line 2 lt 2 lw 2 lc rgb "blue"  # Пунктирная линия
set style line 3 lt 3 lw 3 lc rgb "green"  # Пунктирная линия

plot "data/all_mesurements.txt" using 1:2 title "Стандартный алгоритм умножения матриц" with lines linestyle 1, \
     "" using 1:3 title "Стандартный алгоритм Винограда" with lines linestyle 2, \
     "" using 1:4 title "Оптимизированный алгоритм Винограда" with lines linestyle 3

set terminal svg size 620,480 enhanced
set output 'docs/inc/img/plot.svg'
replot
set output
