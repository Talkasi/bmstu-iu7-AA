.PHONY: clean cleanwpdf

report.pdf: report.tex content.tex title.tex
	pdflatex --shell-escape $<
	biber $(patsubst %.tex,%,$(<))
	pdflatex --shell-escape $<
	pdflatex --shell-escape $<

clean:
	rm -f *.out *.toc *.aux *.log *.bbl *.blg *.bcf *.xml

cleanwpdf: clean
	rm -f *.pdf