if MANDOC_INSTALLED

all-local: detox.1.pdf inline-detox.1.pdf detoxrc.5.pdf detox.tbl.5.pdf

detox.1.pdf: detox.1
	$(MANDOC) -T pdf detox.1 > detox.1.pdf

inline-detox.1.pdf: inline-detox.1
	$(MANDOC) -T pdf inline-detox.1 > inline-detox.1.pdf

detoxrc.5.pdf: detoxrc.5
	$(MANDOC) -T pdf detoxrc.5 > detoxrc.5.pdf

detox.tbl.5.pdf: detox.tbl.5
	$(MANDOC) -T pdf detox.tbl.5 > detox.tbl.5.pdf

endif
