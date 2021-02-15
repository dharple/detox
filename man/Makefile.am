all-local: detox.1.pdf inline-detox.1.pdf detoxrc.5.pdf detox.tbl.5.pdf

detox.1.pdf: detox.1
	man -l -Tps detox.1 | ps2pdf - detox.1.pdf

inline-detox.1.pdf: inline-detox.1
	man -l -Tps inline-detox.1 | ps2pdf - inline-detox.1.pdf

detoxrc.5.pdf: detoxrc.5
	man -l -Tps detoxrc.5 | ps2pdf - detoxrc.5.pdf

detox.tbl.5.pdf: detox.tbl.5
	man -l -Tps detox.tbl.5 | ps2pdf - detox.tbl.5.pdf
