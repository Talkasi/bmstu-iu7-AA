ready/report.pdf: docs/main.pdf
	mkdir -p ./ready
	cp docs/main.pdf ready/main.pdf
	mv ready/main.pdf ready/report.pdf

ready/stud-unit-test-report-prev.json: docs/stud-unit-test-report-prev.json
	mkdir -p ./ready
	cp docs/stud-unit-test-report-prev.json ready/stud-unit-test-report-prev.json

ready/app-cli-debug:
	mkdir -p ./ready
	make -f makefile app-cli-debug

.PHONY: clean
clean:
	make -f makefile clean
	echo OK

all: ready/report.pdf ready/stud-unit-test-report-prev.json ready/app-cli-debug
