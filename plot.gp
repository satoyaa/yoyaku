# 1. ターミナル全体の基本サイズを大きめにする（全体的な底上げ）
set terminal pngcairo font "sans,14" size 800,600 mono
set output 'temp.png'

# 2. 軸ラベルのフォントサイズを個別に指定 (例: 18ポイント)
set xlabel "制限時間" font "sans,18"
set ylabel "評価値" font "sans,18"

# 3. 凡例（データタイトル）のフォントサイズを指定 (例: 16ポイント)
set key at graph 1.0, 0.67 right top font "sans,16"

# 4. 軸の数値（目盛り）のフォントサイズを指定 (例: 12ポイント)
# ラベルだけ大きくすると目盛りが小さく見えてしまうため、調整がおすすめです
set tics font "sans,12"

set grid
set yrange [-50:300]
set xrange [-0.5:5.5]

plot 'data.dat' using 0:2 with linespoints lw 2 pt 7 dt 1 title columnhead(2), \
    'data.dat' using 0:3 with linespoints lw 2 pt 9 dt 2 title columnhead(3)



